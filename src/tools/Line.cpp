#include "Line.h"
#include "EditorTools.h"

Line::Line(int id) : Tool(id) {
	name = "Line";
	description = "Draws lines";
	keyShortcut = "L";
	iconPath = ":/line.png";

	uniform = false;
}

// TODO: Change methods contents
void Line::onMousePressed(QMouseEvent* evt, QPoint& cPos) {
	// Call superclasses implementation
	Tool::onMousePressed(evt, cPos);

	if(currMouseButton != RIGHT && currMouseButton != LEFT)
		return;

	uint col = getColour();

	Painter::drawLine(*canvas->overlay, curr.x(), curr.y(), curr.x(), curr.y(), col, *EditorTools::brush);
}

void Line::onMouseDragged(QMouseEvent* evt, QPoint& cPos) {
	Tool::onMouseDragged(evt, cPos);

	if(currMouseButton != RIGHT && currMouseButton != LEFT)
		return;

	uint col = getColour();

	if(uniform) {
		Painter::drawUniformLine(*canvas->overlay, start.x(), start.y(), curr.x(), curr.y(), col, *EditorTools::brush);
	} else {
		Painter::drawLine(*canvas->overlay, start.x(), start.y(), curr.x(), curr.y(), col, *EditorTools::brush);
	}
}

void Line::onMouseReleased(QMouseEvent* evt, QPoint& cPos) {
	Tool::onMouseReleased(evt, cPos);

	if(currMouseButton != RIGHT && currMouseButton != LEFT)
		return;

	uint col = getColour();

	if(uniform) {
		Painter::drawUniformLine(*canvas->buffer, start.x(), start.y(), curr.x(), curr.y(), col, *EditorTools::brush);
	} else {
		Painter::drawLine(*canvas->buffer, start.x(), start.y(), curr.x(), curr.y(), col, *EditorTools::brush);
	}

	canvas->commit();
}

QList<ToolOptionWidget*> Line::createOptions() {
	return {
		new ToolOptionBool(uniform, [this](QVariant newValue) {
			uniform = newValue.toBool();
		}, "Uniform Line")
	};
}