#include "ProjectView.h"
#include <QGraphicsSimpleTextItem>
#include <QScrollBar>
#include "canvas/ProjectCanvasView.h"
#include <QWheelEvent>
#include "utils/Utils.h"
#include <iostream>

ProjectView::ProjectView(ProjectModel* model, AppModel* appModel) : QGraphicsView(),
	m_Model(model),
	m_AppModel(appModel),
	m_MouseDown(false),
	m_MouseButton(Qt::MouseButton::NoButton),
	m_HasMovedMouse(false),
	m_IgnoreRelease(false),
	m_PrevCanvasCoord(-1, -1) {
	QGraphicsScene* scene = new QGraphicsScene(this);

	// QGraphicsSimpleTextItem* simpleTextItem = scene->addSimpleText("HUGE TEXT LINE AIOAHFHAGOAGNBUABGUOABGUOEBAGUIOAEBNGAGOAOUGOAUNGODIGNAOEUG");
	ProjectCanvasView* item = new ProjectCanvasView(m_Model, m_AppModel);
	scene->addItem(item);

	this->setMouseTracking(true);

	// mouseDown = false;
	// hasMovedMouse = false;
	// ignoreRelease = false;
	// scaleAmt = 1.0;

	// prevCanvasCoord = QPoint(-1, -1);

	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	// this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	this->setResizeAnchor(QGraphicsView::AnchorViewCenter);

	this->setScene(scene);

	item->setPos(
		this->mapToScene(
			this->viewport()->width() / 2.0 - m_Model->surface().width() / 2.0,
			this->viewport()->height() / 2.0 - m_Model->surface().height() / 2.0
		)
	);

	m_CanvasView = item;

	this->connect(model, &ProjectModel::zoomUpdated, this, &ProjectView::setZoom);

	this->connect(appModel, &AppModel::canvasDragModeRequested, this, [this](bool scrollDrag) {
		if(scrollDrag) {
			this->setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
		} else {
			this->setDragMode(QGraphicsView::DragMode::NoDrag);
		}
	});

	this->updateScrollMargins(m_Model->zoom());
}

ProjectView::~ProjectView() {
	delete m_CanvasView;
}

ProjectModel* ProjectView::model() {
	return this->m_Model;
}

void ProjectView::setZoom(float oldZoom, float newZoom, QPointF* zoomOrigin) {
	if(oldZoom == newZoom) { // If not changed don't care
		return;
	}

	float factor = newZoom / m_Model->zoom();

	assert(newZoom != 0);

	if(zoomOrigin == nullptr) {
		// Zoom around centre of canvas

		QSizeF cSizeOld = m_Model->surface().size().toSizeF() * oldZoom;
		QSizeF cSizeNew = m_Model->surface().size().toSizeF() * newZoom;

		float widthDiff = cSizeNew.width() - cSizeOld.width();
		float heightDiff = cSizeNew.height() - cSizeOld.height();

		float newX = m_CanvasView->pos().x() - widthDiff / 2.0;
		float newY = m_CanvasView->pos().y() - heightDiff / 2.0;

		m_CanvasView->setScale(newZoom);
		m_CanvasView->setPos(newX, newY);
		this->updateScrollMargins(newZoom);
	} else {
		// Zoom around origin

		QPointF originPos = this->mapToScene(zoomOrigin->toPoint());
		QPointF itemPos = m_CanvasView->pos();

		QPointF toItem = (itemPos - originPos);
        // float toItemLen = sqrt(toItem.x() * toItem.x() + toItem.y() * toItem.y());
		QPointF toItemScaled = toItem * factor;
        // float toItemScaledLen = sqrt(toItemScaled.x() * toItemScaled.x() + toItemScaled.y() * toItemScaled.y());

		QPointF newItemPos = toItemScaled + originPos;

		m_CanvasView->setPos(newItemPos);
		m_CanvasView->setScale(newZoom);
		this->updateScrollMargins(newZoom);
	}

	// TODO: Investigate other downscaling algorithms - Aseprite's downscaling algorithm (bicubic?) seemingly preserves more pixel information than the QPainter::SmoothPixmapTransform
	//       I don't know if it's possible to implement and use other scaling algorithms in Qt, but if I can a start might be the Lanczos filter
	//       Look at QGraphicsEffect, this seems like it might work
	//       Could always blur the canvas when it's scaled down, and still use QPainter::SmoothPixmapTransform - that might give better detail? Might be expensive though
	if(newZoom < 1) {
		// If we are zoomed OUT, then smooth the drawn image
		this->setRenderHint(QPainter::SmoothPixmapTransform, true);
	} else {
		// If we are zoomed IN, then definitely don't smooth the drawn image
		this->setRenderHint(QPainter::SmoothPixmapTransform, false);
	}

	this->update();
}

void ProjectView::wheelEvent(QWheelEvent* event) {
	bool zoomIn = event->angleDelta().y() > 0;

	QPointF mousePos = event->position();
	m_Model->stepZoom(zoomIn, &mousePos);
}

void ProjectView::resizeEvent(QResizeEvent* event) {
	this->updateScrollMargins(m_Model->zoom());
	this->update();
}

void ProjectView::mousePressEvent(QMouseEvent *event) { // TODO: Move all this logic and etc. into AppModel?
	QGraphicsView::mousePressEvent(event);

	QPoint pos = this->mapToCanvas(event->pos());
	AbstractTool* currentTool = m_AppModel->currentTool();

	if(currentTool->usageType() == ToolUsageType::None) {
		return;
	}

	if(m_MouseDown) {
		// if(currentTool->usageType() == ToolUsageType::Drag) {
		std::cerr << "Tool cancel" << std::endl;
		currentTool->onDrag(m_Model->surface(), m_Model->buffer(), pos, event->button(), ToolDragState::Cancel, m_AppModel);
		m_Model->revertBuffer();
		m_CanvasView->update();
		// }
		m_IgnoreRelease = true;
		// m_MouseDown = false;
	} else {
		if(currentTool->usageType() == ToolUsageType::Drag) {
			std::cerr << "Tool drag press" << std::endl;
			currentTool->onDrag(m_Model->surface(), m_Model->buffer(), pos, event->button(), ToolDragState::Press, m_AppModel);
		}
		if(currentTool->usageType() == ToolUsageType::Click) {
			std::cerr << "Tool click" << std::endl;
			currentTool->onClick(m_Model->surface(), m_Model->buffer(), pos, event->button(), m_AppModel);
		}
		m_MouseDown = true;
		m_MouseButton = event->button();
		m_IgnoreRelease = false;
	}

	m_HasMovedMouse = false;

	m_CanvasView->update();
}

void ProjectView::mouseReleaseEvent(QMouseEvent *event) {
	QGraphicsView::mouseReleaseEvent(event);

	if(event->button() != m_MouseButton) {
		return;
	}

	QPoint pos = this->mapToCanvas(event->pos());
	AbstractTool* currentTool = m_AppModel->currentTool();

	if(currentTool->usageType() == ToolUsageType::None) {
		return;
	}

	if(!m_IgnoreRelease) {
		if(currentTool->usageType() == ToolUsageType::Drag) {
			std::cerr << "Tool drag release" << std::endl;
			currentTool->onDrag(m_Model->surface(), m_Model->buffer(), pos, m_MouseButton, ToolDragState::Release, m_AppModel);
			m_Model->commitBuffer();
			m_CanvasView->update();
		} else if(currentTool->usageType() == ToolUsageType::Click) {
			m_Model->commitBuffer();
		}
		//  else if(!m_HasMovedMouse && currentTool->usageType() == ToolUsageType::Click) {
		// 	std::cerr << "Tool click" << std::endl;
		// 	currentTool->onClick(m_Model->buffer(), pos, m_MouseButton, m_AppModel);
		// 	m_Model->commitBuffer();
		// 	m_CanvasView->update();
		// }
	}

	m_MouseDown = false;
	m_HasMovedMouse = false;
	m_MouseButton = Qt::MouseButton::NoButton;
}

void ProjectView::mouseMoveEvent(QMouseEvent *event) {
	QGraphicsView::mouseMoveEvent(event);

	QPoint pos = this->mapToCanvas(event->pos());
	AbstractTool* currentTool = m_AppModel->currentTool();

	if(currentTool->usageType() == ToolUsageType::None) {
		return;
	}

	if(m_PrevCanvasCoord != pos) {
		if(m_MouseDown && !m_IgnoreRelease && currentTool->usageType() == ToolUsageType::Drag) {
			std::cerr << "Tool drag" << std::endl;
			currentTool->onDrag(m_Model->surface(), m_Model->buffer(), pos, m_MouseButton, ToolDragState::Drag, m_AppModel);
			m_CanvasView->update();
		}

		m_HasMovedMouse = true;
		m_PrevCanvasCoord = pos;
	}
}

void ProjectView::updateScrollMargins(float withZoom) {
	QPointF cPos = m_CanvasView->pos();
	QSizeF cSize = m_Model->surface().size().toSizeF() * withZoom;

	QSizeF vpSize = this->viewport()->size().toSizeF();

	// Calculate margins (offsets of scene rect from canvas)
	float marginHor = utils::constrain<float>(vpSize.width() - cSize.width(), vpSize.width() / 2.0, vpSize.width());
	float marginVer = utils::constrain<float>(vpSize.height() - cSize.height(), vpSize.height() / 2.0, vpSize.height());

	float sceneWidth = marginHor * 2.0 + cSize.width();
	float sceneHeight = marginVer * 2.0 + cSize.height();

	float sceneX = cPos.x() - marginHor;
	float sceneY = cPos.y() - marginVer;

	this->setSceneRect(
		sceneX,
		sceneY,
		sceneWidth,
		sceneHeight
	);
}

QPoint ProjectView::mapToCanvas(const QPoint& pt) {
	QPointF posF = m_CanvasView->mapFromScene(this->mapToScene(pt));
	QPoint pos = QPoint(
		(int)floor(posF.x()),
		(int)floor(posF.y())
	);

	return pos;
}