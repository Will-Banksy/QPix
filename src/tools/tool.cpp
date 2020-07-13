#include "tool.h"
#include "src/editortools.h"

Tool::Tool(int id, Canvas* canvas) {
	this->id = id;
	settings = ToolSettings();
	this->canvas = canvas;
}

Tool::~Tool() {
}

void Tool::onMousePressed(QMouseEvent* evt, QPoint& cPos) {
	firstDragEvent = true;
	curr = QPoint(cPos);
	currMouseButton = getMouseButton(evt);
	start = QPoint(cPos);
}

void Tool::onMouseReleased(QMouseEvent* evt, QPoint& cPos) {
	canvas->overlay->fill(0x00000000);
	if(firstDragEvent)
		prev = QPoint(start);
	else
		prev = QPoint(curr);
	curr = QPoint(cPos);
	currMouseButton = getMouseButton(evt);
	firstDragEvent = false;
}

void Tool::onMouseClicked(QMouseEvent* evt, QPoint& cPos) {
	curr = QPoint(cPos);
	currMouseButton = getMouseButton(evt);
	prev = QPoint(curr);
	start = QPoint(prev);
}

void Tool::onMouseDragged(QMouseEvent* evt, QPoint& cPos) {
	canvas->overlay->fill(0x00000000);
	if(firstDragEvent)
		prev = QPoint(start);
	else
		prev = QPoint(curr);
	curr = QPoint(cPos);
// 	currMouseButton = getMouseButton(evt); // QMouseEvent.button() is always Qt::NoButton for mouse move events
	firstDragEvent = false;
}

void Tool::onDragCancel(QMouseEvent* evt, QPoint& cPos) {
	if(firstDragEvent)
		prev = QPoint(start);
	else
		prev = QPoint(curr);
	curr = QPoint(cPos);
	currMouseButton = getMouseButton(evt); // Set to mouse button that cancelled the drag
	firstDragEvent = false;

	canvas->revert();
}

void Tool::onMouseMoved(QMouseEvent* evt, QPoint& cPos) {
	curr = QPoint(cPos);
	currMouseButton = NONE;
}

Tool::MouseButton Tool::getMouseButton(QMouseEvent* evt) {
	switch(evt->button()) {
		default:
			return NONE;
		case Qt::RightButton:
			return RIGHT;
		case Qt::LeftButton:
			return LEFT;
		case Qt::MiddleButton:
			return MIDDLE;
	}
}

uint Tool::getColour() {
	switch(currMouseButton) {
		default:
			return EditorTools::primaryColour;
		case RIGHT:
			return EditorTools::secondaryColour;
	}
}