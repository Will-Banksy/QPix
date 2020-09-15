#include "Pencil.h"
#include <iostream>
#include "EditorTools.h"

// the ': Tool(id, canvas)' in the initialisation list is like calling super(id, tool)
Pencil::Pencil(int id, Canvas* canvas) : Tool(id, canvas) {
	name = "Pencil";
	description = "Draws pixels";
	keyShortcut = "P";
	iconPath = ":/pencil.png";
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

	Painter::drawLine(*canvas->buffer, curr.x(), curr.y(), curr.x(), curr.y(), col, EditorTools::brush);
}

void Pencil::onMouseDragged(QMouseEvent* evt, QPoint& cPos) {
	Tool::onMouseDragged(evt, cPos);

	if(currMouseButton != RIGHT && currMouseButton != LEFT)
		return;

	uint col = getColour();

	if(settings.pixelPerfect) {
		// Need function to call here
	} else {
		Painter::drawLine(*canvas->buffer, curr.x(), curr.y(), prev.x(), prev.y(), col, EditorTools::brush);
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
		Painter::drawLine(*canvas->buffer, prev.x(), curr.y(), prev.x(), prev.y(), col, EditorTools::brush);
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
