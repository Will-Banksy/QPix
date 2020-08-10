#include "ToolButton.h"
#include <QVariant>
#include <iostream>
#include "EditorTools.h"

ToolButton::ToolButton(Tool& tool, QWidget* parent) : QToolButton(parent), tool(tool) {
	setCheckable(true); // This button gets toggled. Don't even need to do any click handling!
	connect(this, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool)));
	setMinimumSize(40, 40); // Make sure the button won't be resized. If I use SVG's for the icons, I could instead use hasHeightForWidth (something like that)
	setMaximumSize(40, 40);
// 	this->tool = tool;
}

ToolButton::~ToolButton() {
}

void ToolButton::onToggled(bool checked) {
	if(checked) {
		// Here we need to uncheck all other tool buttons and make this one the selected tool
		for(Tool tool : EditorTools::tools) {
			if(&tool != &this->tool) { // If the pointers are unequal - so if the object they are referencing isn't the same one
				tool.button->setChecked(false);
			}
		}
	} else {
		setChecked(true); // Reset it back to checked. This button should not be unchecked the conventional way
	}
}