#include "ToolPane.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include "ui/FlowLayout.h"
#include "tools/Tool.h"
#include "EditorTools.h"
#include "ui/widgets/ToolButton.h"

ToolPane::ToolPane(QWidget* parent) : QDockWidget(parent) {
	setWindowTitle("Tools");

	QVBoxLayout* outerLayout = new QVBoxLayout();
	outerLayout->setAlignment(Qt::AlignTop);

	FlowLayout* flowLayout = new FlowLayout();

	addToolButtons(flowLayout);

	outerLayout->addLayout(flowLayout);

	// Setting the layout directly on the QDockWidget doesn't work, it says in the Qt docs that QDockWidget acts as a wrapper for it's child widget, set with setWidget
	QWidget* widget = new QWidget();
	widget->setProperty("class", "DockContent");
	widget->setLayout(outerLayout);
	setWidget(widget);
}

void ToolPane::addToolButtons(FlowLayout* layout) {
	for(Tool* tool : EditorTools::tools) {
		layout->addWidget(new ToolButton());
	}
}