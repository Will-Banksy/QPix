#include "ToolButton.h"
#include <QVariant>
#include <iostream>

ToolButton::ToolButton(QWidget* parent) : QPushButton(parent) {
	connect(this, SIGNAL(toggled()), this, SLOT(onClicked()));
	setCheckable(true);
	setProperty("class", "ToolButton");
}

ToolButton::~ToolButton() {
}

void ToolButton::onToggled(bool checked) {
	// React somehow
}
