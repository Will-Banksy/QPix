#include "RectangularSelect.h"

RectangularSelect::RectangularSelect(int id) : Tool(id) {
	name = "Rectangular Select";
	description = "Selects a rectangular region";
	keyShortcut = "M";
	iconPath = ":/pencil.png";
}

// TODO

void RectangularSelect::onMousePressed(QMouseEvent* evt, QPoint& cPos) {
}

void RectangularSelect::onMouseDragged(QMouseEvent* evt, QPoint& cPos) {
}

void RectangularSelect::onMouseClicked(QMouseEvent* evt, QPoint& cPos) {
}

void RectangularSelect::onMouseReleased(QMouseEvent* evt, QPoint& cPos) {
}

bool RectangularSelect::isSelector() {
	return true;
}
