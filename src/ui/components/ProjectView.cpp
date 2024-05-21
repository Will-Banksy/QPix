#include "ProjectView.h"
#include <QGraphicsSimpleTextItem>
#include <QScrollBar>
#include "ProjectCanvasView.h"
#include <QWheelEvent>
#include "utils/Helper.h"

ProjectView::ProjectView(ProjectModel* model, AppModel* appModel) : QGraphicsView(), m_Model(model), m_AppModel(appModel) {
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
			this->viewport()->width() / 2.0 - m_Model->surface()->width() / 2.0,
			this->viewport()->height() / 2.0 - m_Model->surface()->height() / 2.0
		)
	);

	m_CanvasView = item;

	connect(model, &ProjectModel::zoomUpdated, this, &ProjectView::setZoom);

	this->updateScrollMargins(m_Model->zoom());
}

ProjectView::~ProjectView() {
	delete m_CanvasView;
}

ProjectModel* ProjectView::model() {
	return this->m_Model;
}

void ProjectView::setZoom(float oldZoom, float newZoom, QPointF* zoomOrigin) {
	float factor = newZoom / m_Model->zoom();

	assert(newZoom != 0);

	if(zoomOrigin == nullptr) {
		// Zoom around centre of canvas

		this->updateScrollMargins(newZoom);

		QSizeF cSizeOld = m_Model->surface()->size().toSizeF() * oldZoom;
		QSizeF cSizeNew = m_Model->surface()->size().toSizeF() * newZoom;

		float widthDiff = cSizeNew.width() - cSizeOld.width();
		float heightDiff = cSizeNew.height() - cSizeOld.height();

		float newX = m_CanvasView->pos().x() - widthDiff / 2.0;
		float newY = m_CanvasView->pos().y() - heightDiff / 2.0;

		m_CanvasView->setScale(newZoom);
		m_CanvasView->setPos(newX, newY);
	} else {
		// Zoom around origin

		QPointF originPos = this->mapToScene(zoomOrigin->toPoint());
		QPointF itemPos = m_CanvasView->pos();

		QPointF toItem = (itemPos - originPos);
		float toItemLen = sqrt(toItem.x() * toItem.x() + toItem.y() * toItem.y());
		QPointF toItemScaled = toItem * factor;
		float toItemScaledLen = sqrt(toItemScaled.x() * toItemScaled.x() + toItemScaled.y() * toItemScaled.y());

		QPointF newItemPos = toItemScaled + originPos;

		m_CanvasView->setPos(newItemPos);
		this->updateScrollMargins(newZoom);
		m_CanvasView->setScale(newZoom);
	}

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

void ProjectView::updateScrollMargins(float withZoom) {
	// float itemWidth = m_Zoom * m_Model->surface()->width();
	// float itemHeight = m_Zoom * m_Model->surface()->height();

	// m_CanvasView->setPos(
	// 	floor((qreal)this->viewport()->width() / 2.0),
	// 	floor((qreal)this->viewport()->height() / 2.0)
	// );
	// this->setSceneRect(
	// 	(qreal)floor(this->viewport()->width() / 2.0),
	// 	(qreal)floor(this->viewport()->height() / 2.0),
	// 	(float)floor(m_Model->surface()->width() * m_Zoom),
	// 	(float)floor(m_Model->surface()->height() * m_Zoom)
	// );

	// ---

	// float cwidth = m_Zoom * m_Model->surface()->width();
	// float cheight = m_Zoom * m_Model->surface()->height();

	// float halfWidth = this->viewport()->width() / 2.0;
	// float halfHeight = this->viewport()->height() / 2.0;

	// float marginHor = utils::constrain<float>(halfWidth - cwidth, 0, halfWidth) + halfWidth;
	// float marginVer = utils::constrain<float>(halfHeight - cheight, 0, halfHeight) + halfHeight;

	// marginHor /= m_Zoom;
	// marginVer /= m_Zoom;

	// QPointF canvasPosGlobal = this->mapToGlobal(m_CanvasView->pos()); // TODO: And then resore this?

	// QRect sceneRect = QRect( // FIXME: Make the scene rect work nicely - Perhaps simply origin the scene rect on (0, 0) and constrain the Item to those bounds... but the scene rect still kinda fucks things
	// 	// int(-marginHor),
	// 	// int(-marginVer),
	// 	0,
	// 	0,
	// 	int(marginHor * 2) + m_Model->surface()->width(),
	// 	int(marginVer * 2) + m_Model->surface()->height()
	// );

	// std::cout << "Scene Rect: (" << sceneRect.x() << ", " << sceneRect.y() << ", " << sceneRect.width() << ", " << sceneRect.height() << ")" << std::endl;

	// this->setSceneRect(sceneRect);

	// ---

	// this->horizontalScrollBar()->setRange(0, 500);
	// this->verticalScrollBar()->setRange(0, 500);

	// ---

	QPointF cPos = m_CanvasView->pos();
	QSizeF cSize = m_Model->surface()->size().toSizeF() * withZoom;

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