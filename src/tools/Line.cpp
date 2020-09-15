#include "Line.h"
#include "EditorTools.h"

Line::Line(int id, Canvas* canvas) : Tool(id, canvas) {
	name = "Line";
	description = "Draws lines";
	keyShortcut = "L";
	iconPath = ":/line.png";
}

// TODO: Change methods contents
void Line::onMousePressed(QMouseEvent* evt, QPoint& cPos) {
	// Call superclasses implementation
	Tool::onMousePressed(evt, cPos);

	if(currMouseButton != RIGHT && currMouseButton != LEFT)
		return;

	uint col = getColour();

	Painter::drawLine(*canvas->overlay, curr.x(), curr.y(), curr.x(), curr.y(), col, EditorTools::brush);
}

void Line::onMouseDragged(QMouseEvent* evt, QPoint& cPos) {
	Tool::onMouseDragged(evt, cPos);

	if(currMouseButton != RIGHT && currMouseButton != LEFT)
		return;

	uint col = getColour();

	if(settings.oneToOneRatio) {
		// Need function to call here
	} else {
		Painter::drawLine(*canvas->overlay, start.x(), start.y(), curr.x(), curr.y(), col, EditorTools::brush);
	}
}

void Line::onMouseReleased(QMouseEvent* evt, QPoint& cPos) {
	Tool::onMouseReleased(evt, cPos);

	if(currMouseButton != RIGHT && currMouseButton != LEFT)
		return;

	uint col = getColour();

	if(settings.oneToOneRatio) {
		// Need function to call here
	} else {
		Painter::drawLine(*canvas->buffer, start.x(), start.y(), curr.x(), curr.y(), col, EditorTools::brush);
	}

	canvas->commit();
}