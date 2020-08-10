#include "ToolButton.h"
#include <QVariant>
#include <iostream>

ToolButton::ToolButton(Tool* tool, QWidget* parent) : QPushButton(parent) {
	connect(this, SIGNAL(toggled()), this, SLOT(onToggled()));
	setCheckable(true); // This button gets toggled. Don't even need to do any click handling!
// 	setProperty("class", "ToolButton"); // Much like assigning a class in HTML - Can target in QSS with .<classname>, for this .ToolButton
	setMinimumSize(40, 40);
	setMaximumSize(40, 40);
	this->tool = tool;
}

ToolButton::~ToolButton() {
}

void ToolButton::onToggled() {
	// React somehow
	std::cout << "Size: w=" << size().width() << ", h=" << size().height() << std::endl;
}