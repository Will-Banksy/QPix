#include "Tool.h"
#include "EditorTools.h"
#include <QShortcut>
#include "Application.h"

Tool::Tool(int id) {
	this->id = id;
	settings = ToolSettings();
}

Tool::~Tool() {
}

void Tool::onMousePressed(QMouseEvent* evt, QPoint& cPos) {
	firstDragEvent = true;
	curr = QPoint(cPos);
	currMouseButton = getMouseButton(evt);
	start = QPoint(cPos);
	EditorTools::toolSwitchlock = true;
// 	std::cout << "onMousePressed" << std::endl;
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
	if(evt->buttons() == Qt::NoButton) {
		EditorTools::toolSwitchlock = false;
	}
// 	std::cout << "onMouseReleased" << std::endl;
}

void Tool::onMouseClicked(QMouseEvent* evt, QPoint& cPos) {
	curr = QPoint(cPos);
	currMouseButton = getMouseButton(evt);
	prev = QPoint(curr);
	start = QPoint(prev);
// 	std::cout << "onMouseClicked" << std::endl;
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
// 	std::cout << "onMouseDragged" << std::endl;
}

void Tool::onDragCancel(QMouseEvent* evt, QPoint& cPos) {
	canvas->overlay->fill(0x00000000);
	if(firstDragEvent)
		prev = QPoint(start);
	else
		prev = QPoint(curr);
	curr = QPoint(cPos);
	currMouseButton = getMouseButton(evt); // Set to mouse button that cancelled the drag
	firstDragEvent = false;

	canvas->revert();
	EditorTools::toolSwitchlock = false;
// 	std::cout << "onDragCancel" << std::endl;
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

QList<ToolOptionWidget*> Tool::createOptions() {
	return QList<ToolOptionWidget*>();
}

bool Tool::isSelector() {
	return false;
}
