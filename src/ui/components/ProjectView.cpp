#include "ProjectView.h"
#include <QGraphicsSimpleTextItem>
#include <iostream>
#include <QScrollBar>
#include "ProjectCanvasView.h"
#include <QWheelEvent>
#include "utils/Helper.h"

const int NUM_ZOOM_FACTORS = 19;
const float ZOOM_FACTORS[NUM_ZOOM_FACTORS] = {
	0.01, 0.02, 0.04, 0.08, 0.16, 0.32, 0.5, 0.75,
	1.0,
	1.5, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 12.0, 16.0
};

ProjectView::ProjectView(ProjectModel* model, AppModel* appModel) : QGraphicsView(), m_Model(model), m_AppModel(appModel), m_Zoom(1.0) {
	QGraphicsScene* scene = new QGraphicsScene(this);

	// QGraphicsSimpleTextItem* simpleTextItem = scene->addSimpleText("HUGE TEXT LINE AIOAHFHAGOAGNBUABGUOABGUOEBAGUIOAEBNGAGOAOUGOAUNGODIGNAOEUG");
	ProjectCanvasView* item = new ProjectCanvasView(m_Model, m_AppModel);
	scene->addItem(item);

	std::cerr << "ProjectView" << std::endl;

	this->setMouseTracking(true);

	// mouseDown = false;
	// hasMovedMouse = false;
	// ignoreRelease = false;
	// scaleAmt = 1.0;

	// prevCanvasCoord = QPoint(-1, -1);

	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	this->setResizeAnchor(QGraphicsView::AnchorViewCenter);

	this->setScene(scene);

	m_CanvasView = item;
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

	// float factor = 1;
	// if(event->angleDelta().y() > 0) {
	// 	factor = 1.1;
	// } else {
	// 	factor = 0.9;
	// }

	// float newZoom = m_Zoom * factor;

	if(newZoom < 1) {
		// If we are zoomed OUT, then smooth the drawn image
		this->setRenderHint(QPainter::SmoothPixmapTransform, true);
	} else {
		// If we are zoomed IN, then definitely don't smooth the drawn image
		this->setRenderHint(QPainter::SmoothPixmapTransform, false);
	}

	// this->resetTransform();
	// this->setTransform(QTransform::fromScale(newZoom, newZoom));
	// this->scale(newZoom / m_Zoom, newZoom / m_Zoom);
	// this->scale(factor, factor);

	QPointF cursorPos = event->position();

	this->updateScrollMargins();

	this->update();

	m_Zoom = newZoom;
}

void ProjectView::resizeEvent(QResizeEvent* event) {
	this->updateScrollMargins();
	this->update();
}

void ProjectView::updateScrollMargins() {
	float cwidth = m_Zoom * m_Model->surface()->width();
	float cheight = m_Zoom * m_Model->surface()->width();

	float halfWidth = this->viewport()->width() / 2.0;
	float halfHeight = this->viewport()->height() / 2.0;

	float marginHor = utils::constrain<float>(halfWidth - cwidth, 0, halfWidth) + halfWidth;
	float marginVer = utils::constrain<float>(halfHeight - cheight, 0, halfHeight) + halfHeight;

	marginHor /= m_Zoom;
	marginVer /= m_Zoom;

	this->setSceneRect(QRect(
		int(-marginHor),
		int(-marginVer),
		int(marginHor * 2) + m_Model->surface()->width(),
		int(marginVer * 2) + m_Model->surface()->height()
	));
}