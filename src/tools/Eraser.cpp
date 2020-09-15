#include "Eraser.h"
#include "EditorTools.h"

// the ': Tool(id, canvas)' in the initialisation list is like calling super(id, tool)
Eraser::Eraser(int id, Canvas* canvas) : Tool(id, canvas) {
	name = "Eraser";
	description = "Erases pixels";
	keyShortcut = "E";
	iconPath = ":/eraser.png";
}

Eraser::~Eraser() {
}

void Eraser::onMousePressed(QMouseEvent* evt, QPoint& cPos) {
	// Call superclasses implementation
	Tool::onMousePressed(evt, cPos);

	CHECK_MOUSE_BUTTONS

	Painter::drawLine(*canvas->buffer, curr.x(), curr.y(), curr.x(), curr.y(), transparent, EditorTools::brush);
}

void Eraser::onMouseDragged(QMouseEvent* evt, QPoint& cPos) {
	Tool::onMouseDragged(evt, cPos);

	CHECK_MOUSE_BUTTONS

	Painter::drawLine(*canvas->buffer, curr.x(), curr.y(), prev.x(), prev.y(), transparent, EditorTools::brush);
}

void Eraser::onMouseReleased(QMouseEvent* evt, QPoint& cPos) {
	Tool::onMouseReleased(evt, cPos);

	CHECK_MOUSE_BUTTONS

	Painter::drawLine(*canvas->buffer, prev.x(), curr.y(), prev.x(), prev.y(), transparent, EditorTools::brush);

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
