#include "eraser.h"

// the ': Tool(id, canvas)' in the initialisation list is like calling super(id, tool)
Eraser::Eraser(int id, Canvas* canvas) : Tool(id, canvas) {
	name = "Eraser";
	description = "Erases pixels";
	keyShortcut = "E";
	iconPath = ":/Eraser.png";

	// Assign the lambda (capturing the 'this' pointer so I can use global variables) to the std::function
	toolAction = [&, this](int i, int j, uint col, ToolSettings& settings) {
		this->canvas->buffer->setPixel(i, j, col);
	};
}

Eraser::~Eraser() {
}

void Eraser::onMousePressed(QMouseEvent* evt, QPoint& cPos) {
	// Call superclasses implementation
	Tool::onMousePressed(evt, cPos);

	if(currMouseButton != RIGHT && currMouseButton != LEFT)
		return;

	Algorithms::plotLine(curr.x(), curr.y(), curr.x(), curr.y(), transparent, settings, toolAction);
}

void Eraser::onMouseDragged(QMouseEvent* evt, QPoint& cPos) {
	Tool::onMouseDragged(evt, cPos);

	if(currMouseButton != RIGHT && currMouseButton != LEFT)
		return;

	Algorithms::plotLine(curr.x(), curr.y(), prev.x(), prev.y(), transparent, settings, toolAction);
}

void Eraser::onMouseReleased(QMouseEvent* evt, QPoint& cPos) {
	Tool::onMouseReleased(evt, cPos);

	if(currMouseButton != RIGHT && currMouseButton != LEFT)
		return;

	Algorithms::plotLine(prev.x(), curr.y(), prev.x(), prev.y(), transparent, settings, toolAction);

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
