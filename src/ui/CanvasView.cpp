#include "CanvasView.h"
#include <QMouseEvent>
#include "EditorTools.h"
#include "ProjectScene.h"
// #include <QOpenGLWidget>
#include <Project.h>
#include <cmath>
#include "utils/Utils.h"
#include <QScrollBar>

CanvasView::CanvasView(QWidget* parent) : QGraphicsView(parent) {
// 	setViewport(new QOpenGLWidget()); // Rendering using a QOpenGLWidget does reduce cpu usage - but also of course increases gpu usage. Anyway what I need to do is make it more efficient, not this.

	// Track mouse to receive mouse moved events
	setMouseTracking(true);

	mouseDown = false;
	hasMovedMouse = false;
	ignoreRelease = false;
	scaleAmt = 1.0;

	prevCanvasCoord = QPoint(-1, -1);

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	connect(horizontalScrollBar(), &QScrollBar::valueChanged, this, [this](int val) {
		if(scene()) {
			((ProjectScene*)scene())->project->scrollPos.setX(val);
		}
	});
	connect(verticalScrollBar(), &QScrollBar::valueChanged, this, [this](int val) {
		if(scene()) {
			((ProjectScene*)scene())->project->scrollPos.setY(val);
		}
	});
}

CanvasView::~CanvasView() {
}

void CanvasView::wheelEvent(QWheelEvent* event) { // Also should make it so you can only zoom out so far
	if(!scene()) return;

	float factor = 1;
	if(event->angleDelta().y() > 0) {
		factor = 1.1;
	} else {
		factor = 0.9;
	}
	// Scaling bounds: x120 - x0.1
	factor = scaleAmt * factor > 180 ? 180.0 / scaleAmt : factor; // Needs to be long-time-useage-proofed - need some way to get the current scale - current tracking method may be subject to gradual divergence from the real scale
	factor = scaleAmt * factor < 0.1 ? 0.1 / scaleAmt : factor; // Needs to be long-time-useage-proofed - need some way to get the current scale - current tracking method may be subject to gradual divergence from the real scale
	scaleAmt *= factor;

	if(scaleAmt < 1) {
		setRenderHint(QPainter::SmoothPixmapTransform, true); // If we are zoomed OUT, then smooth the drawn image
	} else {
		setRenderHint(QPainter::SmoothPixmapTransform, false); // If we are zoomed IN, then definitely don't smooth the drawn image
	}

	scale(factor, factor);

	updateScrollMargins();

	update();

	// Update the transform stored by the project
	((ProjectScene*)scene())->project->viewTransform.scale(factor, factor);
	((ProjectScene*)scene())->project->viewScaleAmt = scaleAmt;
}

void CanvasView::mousePressEvent(QMouseEvent* event) {
	if(!scene()) return;

	QPoint canvasPos = mapToCanvas(event->pos());

	EditorTools::selectedTool->canvas = ((ProjectScene*)scene())->canvas;

	if(mouseDown) {
		EditorTools::selectedTool->onDragCancel(event, canvasPos);
		ignoreRelease = true;
		mouseDown = false;
	} else {
		EditorTools::selectedTool->onMousePressed(event, canvasPos);
		mouseDown = true;
		ignoreRelease = false;
	}

	hasMovedMouse = false;

	((ProjectScene*)scene())->canvas->update();

// 	QRectF sr = sceneRect();
// 	std::cout << "SceneRect: (x:" << sr.x() << ",y:" << sr.y() << ",w:" << sr.width() << ",h:" << sr.height() << std::endl;

	QGraphicsView::mousePressEvent(event);
}

void CanvasView::mouseReleaseEvent(QMouseEvent* event) {
	if(!scene()) return;

	QPoint canvasPos = mapToCanvas(event->pos());

	if(!ignoreRelease) {
		if(!hasMovedMouse) {
			EditorTools::selectedTool->onMouseClicked(event, canvasPos);
		}
		EditorTools::selectedTool->onMouseReleased(event, canvasPos);
	}

	mouseDown = false;
	hasMovedMouse = false;

	((ProjectScene*)scene())->canvas->update();
// 	EditorTools::selectedTool->canvas = nullptr;

	QGraphicsView::mouseReleaseEvent(event);
}

void CanvasView::mouseMoveEvent(QMouseEvent* event) {
	if(!scene()) return;

	QGraphicsView::mouseMoveEvent(event); // Need to call superclass here (otherwise the transformation anchor will be ignored)

	QPoint canvasPos = mapToCanvas(event->pos());

	if(prevCanvasCoord != canvasPos) { // So the canvas will only be repainted when the mouse moves to a new pixel. This makes things much less demanding
		if(mouseDown) {
			EditorTools::selectedTool->onMouseDragged(event, canvasPos);
		} else {
			EditorTools::selectedTool->onMouseMoved(event, canvasPos);
		}

		hasMovedMouse = true;

		((ProjectScene*)scene())->canvas->update();
		prevCanvasCoord = canvasPos;
	}
}

// void CanvasView::updateVisible() {
// 	QPointF upperLeft = mapToScene(QPoint(0, 0));
// 	QPointF lowerRight = mapToScene(QPoint(width(), height()));
// }

void CanvasView::paintEvent(QPaintEvent* event) {
	event->ignore();
	QGraphicsView::paintEvent(event);
	QPainter painter(this->viewport());
	painter.setPen(QPen(QColor(0xff000000), 2));

// 	QPointF p1 = mapFromCanvas(0, 0);
// 	p1.setX(p1.x() - 1.5);
// 	p1.setY(p1.y() - 1.5);
//
// 	QPointF p2 = mapFromCanvas(0, ((ProjectScene*)scene())->canvas->buffer->height());
// 	p2.setX(p2.x() - 1.5);
// 	p2.setY(p2.y() + 1);
//
// 	QPointF p3 = mapFromCanvas(((ProjectScene*)scene())->canvas->buffer->width(), 0);
// 	p3.setX(p3.x() + 1);
// 	p3.setY(p3.y() - 1.5);
//
// 	QPointF p4 = mapFromCanvas(((ProjectScene*)scene())->canvas->buffer->width(), ((ProjectScene*)scene())->canvas->buffer->height());
// 	p4.setX(p4.x() + 1);
// 	p4.setY(p4.y() + 1);
//
// 	painter.drawLine(p1, p2);
// 	painter.drawLine(p1, p3);
// 	painter.drawLine(p2, p4);
// 	painter.drawLine(p3, p4);

	// TODO Use this for drawing stuff over the canvas, like selection stuff mayhaps. Or I might use a widget overlaying this actually. Or I could just redirect all mouse events to a selection handler or whatnot. Hmm
}

QPoint CanvasView::mapToCanvas(const QPoint& pt) {
	return mapToCanvas(pt.x(), pt.y());
}

QPoint CanvasView::mapToCanvas(int x, int y) {
	QPointF pf = mapToScene(x, y);
	return QPoint(floor(pf.x()), floor(pf.y()));
}

QPoint CanvasView::mapFromCanvas(const QPoint& pt) {
	return mapFromCanvas(pt.x(), pt.y());
}

QPoint CanvasView::mapFromCanvas(int x, int y) {
	QPointF pf = mapFromScene(x, y);
	return QPoint(floor(pf.x()), floor(pf.y()));
}

void CanvasView::updateScrollMargins() { // A tad dodgy but it works. Not seamlessly but it works. From general use it'll appear pretty decent. TODO Make more seamless
	if(scene()) {
		float cwidth = scaleAmt * ((ProjectScene*)scene())->canvas->buffer->width();
		float cheight = scaleAmt * ((ProjectScene*)scene())->canvas->buffer->height();
		float halfThisWidth = (viewport()->width() / 2);
		float halfThisHeight = (viewport()->height() / 2);
		int marginHor = utils::constrain<int>(halfThisWidth - cwidth, 0, halfThisWidth) + halfThisWidth;
		int marginVer = utils::constrain<int>(halfThisHeight - cheight, 0, halfThisHeight) + halfThisHeight;
		marginHor /= scaleAmt;
		marginVer /= scaleAmt;
// 		std::cout << "scaleAmt: " << scaleAmt << ", cwidth: " << cwidth << ", marginHor: " << marginHor << ", marginVer: " << marginVer << std::endl;
		setSceneRect(QRect(-marginHor, -marginVer, (marginHor * 2) + ((ProjectScene*)scene())->canvas->buffer->width(), (marginVer * 2) + ((ProjectScene*)scene())->canvas->buffer->height()));
	} else {
		setSceneRect(QRect(0, 0, 0, 0));
	}
}

void CanvasView::resizeEvent(QResizeEvent* event) {
	updateScrollMargins();
	update();
}
