#include "ToolButton.h"
#include <QVariant>
#include <iostream>
#include "EditorTools.h"
#include <QPainter>

ToolButton::ToolButton(Tool* tool, QWidget* parent) : QToolButton(parent) {
	setCheckable(true); // This button gets toggled. Don't even need to do any click handling!
	connect(this, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool)));
	setMinimumSize(40, 40); // Make sure the button won't be resized. If I use SVG's for the icons, I could instead use hasHeightForWidth (something like that)
	setMaximumSize(40, 40);

	setAutoExclusive(true); // Act like a radio button, group with all other toolbuttons with the same parent

	this->tool = tool;
	setIcon(QIcon(tool->iconPath));
	setIconSize(QSize(24, 24)); // Set size of the icons so they're not automatically sized
	setShortcut(QKeySequence(this->tool->keyShortcut)); // TODO: Disable shortcuts if we have the mouse pressed on the canvas
	setStatusTip(QString(tool->name).append(" - ").append(tool->description));
}

ToolButton::~ToolButton() {
}

void ToolButton::onToggled(bool checked) {
	if(checked) {
		if(EditorTools::selectedTool != tool) {
			EditorTools::switchTool(tool->id);
		}
	}
}