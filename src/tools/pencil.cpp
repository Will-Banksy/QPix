#include "pencil.h"
#include <iostream>

// the ': Tool(id, canvas)' in the initialisation list is like calling super(id, tool)
Pencil::Pencil(int id, Canvas* canvas) : Tool(id, canvas) {
	name = "Pencil";
	description = "Draws pixels";
	keyShortcut = "P";
	iconPath = ":/Pencil.png";

	// Assign the lambda (capturing the 'this' pointer so I can use global variables) to the std::function
	toolAction = [&, this](int i, int j, uint col, ToolSettings& settings) {
		this->canvas->buffer->setPixel(i, j, col);
	};
}

Pencil::~Pencil() {
}


void Pencil::onMousePressed(QMouseEvent* evt, QPoint& cPos) {
	// Call superclasses implementation
	Tool::onMousePressed(evt, cPos);

	if(currMouseButton != RIGHT && currMouseButton != LEFT)
		return;

	currentStroke = QList<QPair<QPoint, uint>>();

	uint col = getColour();

	Algorithms::plotLine(curr.x(), curr.y(), curr.x(), curr.y(), col, settings, toolAction);
}

void Pencil::onMouseDragged(QMouseEvent* evt, QPoint& cPos) {
	Tool::onMouseDragged(evt, cPos);

	if(currMouseButton != RIGHT && currMouseButton != LEFT)
		return;

	uint col = getColour();

	if(settings.pixelPerfect) {
		// Need function to call here
	} else {
		Algorithms::plotLine(curr.x(), curr.y(), prev.x(), prev.y(), col, settings, toolAction);
	}
}

void Pencil::onMouseReleased(QMouseEvent* evt, QPoint& cPos) {
	Tool::onMouseReleased(evt, cPos);

	if(currMouseButton != RIGHT && currMouseButton != LEFT)
		return;

	uint col = getColour();

	if(settings.pixelPerfect) {
		// Need function to call here
	} else {
		Algorithms::plotLine(prev.x(), curr.y(), prev.x(), prev.y(), col, settings, toolAction);
	}

	canvas->commit();
}

// @Override public void onMouseReleased(MouseEvent me)
// {
// 	super.onMouseReleased(me);
//
// 	if(currMouseButton != MouseButton.RIGHT && currMouseButton != MouseButton.LEFT)
// 		return;
//
// 	int col = getColour();
//
// 	if(settings.pixelPerfect)
// 	{
// 		PaintHandler.drawLine_PixelPerfect(prev, curr, col, DrawTo.CANVAS, EditorTools.brushSize, settings, currentStroke);
// 	}
// 	else
// 	{
// 		PaintHandler.drawLine(prev, curr, col, DrawTo.CANVAS, EditorTools.brushSize, settings);
// 	}
// }
