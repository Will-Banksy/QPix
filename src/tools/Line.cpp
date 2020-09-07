#include "Line.h"

Line::Line(int id, Canvas* canvas) : Tool(id, canvas) {
	name = "Line";
	description = "Draws lines";
	keyShortcut = "L";
	iconPath = ":/line.png";

	// Assign the lambda (capturing the 'this' pointer so I can use global variables) to the std::function
	toolAction = [&, this](int i, int j, uint col, ToolSettings& settings) {
		this->canvas->overlay->setPixel(i, j, col); // TODO: Respect tool settings, brush size, etc.
	};
}

// TODO: Change methods contents
void Line::onMousePressed(QMouseEvent* evt, QPoint& cPos) {
	// Call superclasses implementation
	Tool::onMousePressed(evt, cPos);

	if(currMouseButton != RIGHT && currMouseButton != LEFT)
		return;

	uint col = getColour();

	Algorithms::plotLine(curr.x(), curr.y(), curr.x(), curr.y(), col, settings, toolAction);
}

void Line::onMouseDragged(QMouseEvent* evt, QPoint& cPos) {
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

void Line::onMouseReleased(QMouseEvent* evt, QPoint& cPos) {
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