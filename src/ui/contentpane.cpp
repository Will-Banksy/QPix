#include "contentpane.h"
#include <QVBoxLayout>
#include <QPushButton>
#include "canvaspane.h"

ContentPane::ContentPane(QWidget* parent) : QWidget(parent) {
	QVBoxLayout* layout = new QVBoxLayout();

	QPushButton* btn = new QPushButton("Estoy 'button'");

	CanvasPane* canvasPane = new CanvasPane();

	layout->addWidget(btn);
	layout->addWidget(canvasPane);

	setLayout(layout);
}

ContentPane::~ContentPane() {
}