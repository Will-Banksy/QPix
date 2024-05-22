#include "Fill.h"
#include "EditorTools.h"

Fill::Fill(int id) : Tool(id) {
	name = "Fill";
	description = "Fills an area";
	keyShortcut = "B";
	iconPath = ":/tools/fill.png";

	fill8Way = false;
	tolerance = 0;
}

void Fill::onMouseClicked(QMouseEvent* evt, QPoint& cPos) { // Maybe change this so you can preview and cancel the fill?
	Tool::onMouseClicked(evt, cPos);

	CHECK_MOUSE_BUTTONS

	uint col = getColour();

	Painter::fillArea(*canvas->buffer, curr.x(), curr.y(), col, *EditorTools::brush, tolerance, fill8Way);

	canvas->commit();
}

QList<ToolOptionWidget*> Fill::createOptions() {
	return {
		new ToolOptionBool(fill8Way, [this](QVariant newValue) {
			fill8Way = newValue.toBool();
		}, "Fill 8-Way"),
		new ToolOptionInt(tolerance, [this](QVariant newValue) {
			tolerance = newValue.toInt();
		}, 0, 255)
	};
}