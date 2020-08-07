#include "ContentPane.h"
#include <QVBoxLayout>
#include <QPushButton>
#include "CanvasPane.h"
#include <QSplitter>
#include "ui/WeightedSplitter.h"
#include "ToolPane.h"
#include <iostream>

ContentPane::ContentPane(QWidget* parent) : QWidget(parent) {
	QVBoxLayout* layout = new QVBoxLayout();

// 	QPushButton* btn = new QPushButton("Estoy 'button'");
// 	ToolPane* toolPane = new ToolPane();

	CanvasPane* canvasPane = new CanvasPane();

// 	layout->addWidget(toolPane);
	layout->addWidget(canvasPane);

// 	WeightedSplitter* splitter = new WeightedSplitter(Qt::Horizontal);
// 	splitter->setChildrenCollapsible(false);
// 	splitter->setWeights({0, 1});
// 	splitter->setLayout(layout);
// 	splitter->autoSizeWidgets();
//
// 	QVBoxLayout* layout2 = new QVBoxLayout();
// 	layout2->addWidget(splitter);
	setLayout(layout);
}

ContentPane::~ContentPane() {
}