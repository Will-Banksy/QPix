#include "ProjectView.h"
#include <QGraphicsSimpleTextItem>
#include <QScrollBar>
#include "ProjectCanvasView.h"
#include <QWheelEvent>
#include "utils/Helper.h"

const int NUM_ZOOM_FACTORS = 21;
const float ZOOM_FACTORS[NUM_ZOOM_FACTORS] = {
	0.01, 0.02, 0.04, 0.08, 0.16, 0.32, 0.5, 0.75,
	1.0,
	1.5, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 12.0, 16.0, 32.0, 64.0
};

ProjectView::ProjectView(ProjectModel* model, AppModel* appModel) : QGraphicsView(), m_Model(model), m_AppModel(appModel), m_Zoom(1.0) {
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

	this->updateScrollMargins();
}

ProjectView::~ProjectView() {
}

ProjectModel* ProjectView::model() {
	return this->m_Model;
}

void ProjectView::wheelEvent(QWheelEvent* event) {
	// Calculate the new zoom value
	int zoomI = 0;
	for(int i = 0; i < NUM_ZOOM_FACTORS; i++) {
		if(ZOOM_FACTORS[i] == m_Zoom) {
			zoomI = i;
			break;
		}
	}
	bool zoomIn = event->angleDelta().y() > 0;
	float newZoom = 0;
	if(zoomIn) {
		newZoom = ZOOM_FACTORS[zoomI + 1 >= NUM_ZOOM_FACTORS ? NUM_ZOOM_FACTORS - 1 : zoomI + 1];
	} else {
		if(zoomI != 0) {
			newZoom = ZOOM_FACTORS[zoomI - 1];
		}
	}

	if(newZoom < 1) {
		// If we are zoomed OUT, then smooth the drawn image
		this->setRenderHint(QPainter::SmoothPixmapTransform, true);
	} else {
		// If we are zoomed IN, then definitely don't smooth the drawn image
		this->setRenderHint(QPainter::SmoothPixmapTransform, false);
	}

	float factor = newZoom / m_Zoom;

	QPointF cursorPos = this->mapToScene(event->position().toPoint());
	QPointF itemPos = m_CanvasView->pos();

	QPointF toItem = (itemPos - cursorPos);
	float toItemLen = sqrt(toItem.x() * toItem.x() + toItem.y() * toItem.y());
	QPointF toItemScaled = toItem * factor;
	float toItemScaledLen = sqrt(toItemScaled.x() * toItemScaled.x() + toItemScaled.y() * toItemScaled.y());

	QPointF newItemPos = toItemScaled + cursorPos;

	m_CanvasView->setPos(newItemPos);
	// m_CanvasView->setPos(cursorPos);
	m_CanvasView->setScale(newZoom);

	m_Zoom = newZoom;
	this->updateScrollMargins();

	this->update();
}

void ProjectView::resizeEvent(QResizeEvent* event) {
	this->updateScrollMargins();
	this->update();
}

void ProjectView::updateScrollMargins() {
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
	QSizeF cSize = m_Model->surface()->size().toSizeF() * m_Zoom;

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