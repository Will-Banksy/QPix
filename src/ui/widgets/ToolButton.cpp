#include "ToolButton.h"
#include <QVariant>
#include <iostream>
#include "EditorTools.h"


//
ToolButton::ToolButton(Tool* tool, QWidget* parent) : QToolButton(parent)/*, tool(tool)*/ {
	setCheckable(true); // This button gets toggled. Don't even need to do any click handling!
	connect(this, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool)));
	setMinimumSize(40, 40); // Make sure the button won't be resized. If I use SVG's for the icons, I could instead use hasHeightForWidth (something like that)
	setMaximumSize(40, 40);
	this->tool = tool;
	checkedSafely = false;
	blockNextSignal = false;
}

ToolButton::~ToolButton() {
}

void ToolButton::onToggled(bool checked) {
	if(blockNextSignal) {
		blockNextSignal = false;
		return;
	}
	if(checkedSafely && !checked) {
		checkedSafely = false;
		return;
	}

	if(checked) {
		// Here we need to uncheck the last selected tool button and make this one checked and make this one's tool the selected tool
		if(EditorTools::selectedTool != tool) { // If the pointers are unequal - so if the object they are referencing isn't the same one
			EditorTools::selectedTool->button->setCheckedSafe(false);
			EditorTools::selectedTool = tool;
		}
	} else {
		setCheckedSafe(true, true); // Reset it back to checked. This button should not be unchecked the conventional way
	}
}

void ToolButton::setCheckedSafe(bool checked, bool blockSignal) {
	if(blockSignal) {
		blockNextSignal = true;
	} else {
		checkedSafely = true; // We want two modes: hard block and soft block. Hard block just returns immediately, soft block returns if an 'uncheck' symbol was sent (so that when the last selected tool's button is unchecked, it doesn't start try to re-check itself)
	}
	setChecked(checked);
}
