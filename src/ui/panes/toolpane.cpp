#include "toolpane.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include "src/ui/flowlayout.h"

ToolPane::ToolPane(QWidget* parent) : QDockWidget(parent) {
	setWindowTitle("Tools");

	QVBoxLayout* outerLayout = new QVBoxLayout();
	outerLayout->setAlignment(Qt::AlignTop);

	FlowLayout* flowLayout = new FlowLayout();

	QPushButton* btn1 = new QPushButton();
	QPushButton* btn2 = new QPushButton();

	flowLayout->addWidget(btn1);
	flowLayout->addWidget(btn2);

	outerLayout->addLayout(flowLayout);

	// Setting the layout directly on the QDockWidget doesn't work, it says in the Qt docs that QDockWidget acts as a wrapper for it's child widget, set with setWidget
	QWidget* widget = new QWidget();
	widget->setLayout(outerLayout);
	setWidget(widget);
}