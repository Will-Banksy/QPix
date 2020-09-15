#include "Fill.h"
#include "EditorTools.h"

Fill::Fill(int id, Canvas* canvas) : Tool(id, canvas) {
	name = "Fill";
	description = "Fills an area";
	keyShortcut = "B";
	iconPath = ":/fill.png";
}

void Fill::onMouseClicked(QMouseEvent* evt, QPoint& cPos) { // TODO: Maybe change this so you can preview and cancel the fill?
	Tool::onMouseClicked(evt, cPos);

	CHECK_MOUSE_BUTTONS

	uint col = getColour();

	Painter::fillArea(*canvas->buffer, curr.x(), curr.y(), col, EditorTools::brush);
}

