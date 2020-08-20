#include "ToolPane.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include "tools/Tool.h"
#include "EditorTools.h"
#include "ui/widgets/ToolButton.h"
#include <QSizeGrip>

ToolPane::ToolPane(QWidget* parent) : QDockWidget(parent) {
	setWindowTitle("Tools");

	QVBoxLayout* outerLayout = new QVBoxLayout();
	outerLayout->setAlignment(Qt::AlignTop);

	FlowLayout2* flowLayout = new FlowLayout2();

	addToolButtons(flowLayout);

	outerLayout->addLayout(flowLayout);

	// TODO: Somehow make it so that QDockWidgets are easily resizeable. Currently, you can use the Dock Widget titlebar
	// QSizeGrips might be the way to go
// 	QSizeGrip resizeHandle(this);
// 	resizeHandle.setVisible(true);

	// Setting the layout directly on the QDockWidget doesn't work, it says in the Qt docs that QDockWidget acts as a wrapper for it's child widget, set with setWidget
	QWidget* widget = new QWidget();
	widget->setProperty("class", "DockContent");
	widget->setLayout(outerLayout);
	setWidget(widget);
}

void ToolPane::addToolButtons(FlowLayout2* layout) {
	for(Tool* tool : EditorTools::tools) {
		ToolButton* btn = new ToolButton(tool);
		tool->button = btn;
		if(tool == EditorTools::selectedTool) {
			btn->setCheckedSafe(true, true);
		}
		layout->addWidget(btn);
	}
}