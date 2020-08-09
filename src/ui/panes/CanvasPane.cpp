#include "CanvasPane.h"
#include <QWheelEvent>
#include "EditorTools.h"

CanvasPane::CanvasPane(QWidget* parent) : QGraphicsView(parent) {
	QGraphicsScene* scene = new QGraphicsScene();

	canvas = new Canvas();
	scene->addItem(canvas);

	setScene(scene);

	EditorTools::initTools(canvas);

	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setResizeAnchor(QGraphicsView::AnchorViewCenter);

	// Track mouse to receive mouse moved events
	setMouseTracking(true);

	scaleAmt = 1.0;
}

CanvasPane::~CanvasPane() {
	delete canvas;
}

void CanvasPane::wheelEvent(QWheelEvent* event) {
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
	canvas->update();
}

// This function simply floors floating point values in a way so that flooring -0.5 gives -1 instead of 0
inline int mapFloor(float num) {
	if(num < 0) {
		return (int)(num - 1);
	} else {
		return (int)num;
	}
}

void CanvasPane::mousePressEvent(QMouseEvent* event) {
	QPointF canvasPosF = mapToScene(event->pos());
	QPoint canvasPos = QPoint(mapFloor(canvasPosF.x()), mapFloor(canvasPosF.y()));

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

	canvas->update();
}

void CanvasPane::mouseReleaseEvent(QMouseEvent* event) {
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

	canvas->update();
}

void CanvasPane::mouseMoveEvent(QMouseEvent* event) {
	QGraphicsView::mouseMoveEvent(event); // Need to call superclass here (otherwise the transformation anchor will be ignored)

	QPointF canvasPosF = mapToScene(event->pos());
	QPoint canvasPos = QPoint(mapFloor(canvasPosF.x()), mapFloor(canvasPosF.y()));

	if(mouseDown) {
		EditorTools::selectedTool->onMouseDragged(event, canvasPos);
	} else {
		EditorTools::selectedTool->onMouseMoved(event, canvasPos);
	}

	hasMovedMouse = true;

	canvas->update();
}
