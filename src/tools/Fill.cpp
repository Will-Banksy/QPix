#include "Fill.h"

Fill::Fill(int id, Canvas* canvas) : Tool(id, canvas) {
	name = "Pencil";
	description = "Draws pixels";
	keyShortcut = "P";
	iconPath = ":/Pencil.png";

	// Assign the lambda (capturing the 'this' pointer so I can use global variables) to the std::function
	toolAction = [&, this](int i, int j, uint col, ToolSettings& settings) {
		this->canvas->buffer->setPixel(i, j, col);
	};
}

void Fill::onMouseClicked(QMouseEvent* evt, QPoint& cPos) {
	Tool::onMouseClicked(evt, cPos);

	CHECK_MOUSE_BUTTONS

	uint col = getColour();

	Algorithms::fillArea(*canvas, curr.x(), curr.y(), col, settings, toolAction);
}

