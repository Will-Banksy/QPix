#include "CanvasView.h"
#include <QMouseEvent>
#include "EditorTools.h"
#include "ProjectScene.h"
#include <QOpenGLWidget>

CanvasView::CanvasView(QWidget* parent) : QGraphicsView(parent) {
// 	setViewport(new QOpenGLWidget()); // Rendering using a QOpenGLWidget does reduce cpu usage - but also of course increases gpu usage. Anyway what I need to do is make it more efficient, not this.

	// Track mouse to receive mouse moved events
	setMouseTracking(true);

	mouseDown = false;
	hasMovedMouse = false;
	ignoreRelease = false;
	scaleAmt = 1.0;

	prevCanvasCoord = QPoint(-1, -1);
}

CanvasView::~CanvasView() {
}

void CanvasView::wheelEvent(QWheelEvent* event) { // Also should make it so you can only zoom out so far
	float factor = 1;
	if(event->angleDelta().y() > 0) {
		factor = 1.1;
	} else {
		factor = 0.9;
	}
	factor = scaleAmt * factor > 120 ? 120.0 / scaleAmt : factor; // Needs to be long-time-useage-proofed - need some way to get the current scale - current tracking method may be subject to gradual divergence from the real scale
	scaleAmt *= factor;

	if(scaleAmt < 1) {
		setRenderHint(QPainter::SmoothPixmapTransform, true); // If we are zoomed OUT, then smooth the drawn image
	} else {
		setRenderHint(QPainter::SmoothPixmapTransform, false); // If we are zoomed IN, then definitely don't smooth the drawn image
	}

	scale(factor, factor);
	((ProjectScene*)scene())->canvas->update();
}

// This function simply floors floating point values in a way so that flooring -0.5 gives -1 instead of 0
inline int mapFloor(float num) {
	if(num < 0) {
		return (int)(num - 1); // FIXME Not correct with whole numbers
	} else {
		return (int)num;
	}
}

void CanvasView::mousePressEvent(QMouseEvent* event) {
	QPointF canvasPosF = mapToScene(event->pos());
	QPoint canvasPos = QPoint(mapFloor(canvasPosF.x()), mapFloor(canvasPosF.y()));

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
}

void CanvasView::mouseReleaseEvent(QMouseEvent* event) {
	QPointF canvasPosF = mapToScene(event->pos());
	QPoint canvasPos = QPoint(mapFloor(canvasPosF.x()), mapFloor(canvasPosF.y()));

	if(!ignoreRelease) {
		EditorTools::selectedTool->onMouseReleased(event, canvasPos);
		if(!hasMovedMouse) {
			EditorTools::selectedTool->onMouseClicked(event, canvasPos);
		}
	}

	mouseDown = false;
	hasMovedMouse = false;

	((ProjectScene*)scene())->canvas->update();
// 	EditorTools::selectedTool->canvas = nullptr;
}

void CanvasView::mouseMoveEvent(QMouseEvent* event) {
	QGraphicsView::mouseMoveEvent(event); // Need to call superclass here (otherwise the transformation anchor will be ignored)

	QPointF canvasPosF = mapToScene(event->pos());
	QPoint canvasPos = QPoint(mapFloor(canvasPosF.x()), mapFloor(canvasPosF.y()));

	if(mouseDown) {
		EditorTools::selectedTool->onMouseDragged(event, canvasPos);
	} else {
		EditorTools::selectedTool->onMouseMoved(event, canvasPos);
	}

	hasMovedMouse = true;

	if(prevCanvasCoord != canvasPos) { // So the canvas will only be repainted when the mouse moves to a new pixel. This makes things much less demanding
		((ProjectScene*)scene())->canvas->update();
		prevCanvasCoord = canvasPos;
	}
}

// void CanvasView::updateVisible() {
// 	QPointF upperLeft = mapToScene(QPoint(0, 0));
// 	QPointF lowerRight = mapToScene(QPoint(width(), height()));
// }
