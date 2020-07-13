#include "flowscrollpane.h"
#include <QResizeEvent>

FlowScrollPane::FlowScrollPane(QWidget* parent) : QScrollArea(parent) {
	children = QList<QWidget*>();
}

FlowScrollPane::~FlowScrollPane() {
}

void FlowScrollPane::addWidget(QWidget* widget) {
	children.append(widget);
	widget->setParent(this);
	recalculateMinWidth();
}

void FlowScrollPane::removeWidget(QWidget* widget) {
	children.removeAll(widget);
	recalculateMinWidth();
}

QMargins FlowScrollPane::getMargins() {
	return margins;
}

void FlowScrollPane::setMargins(QMargins margins) {
	this->margins = margins;
	recalculateMinWidth();
}

void FlowScrollPane::recalculateMinWidth() {
	int maxWidth = -1;
	for(QWidget* widget : children) {
		int wwidth = widget->sizeHint().width() + margins.left() + margins.right();
		if(wwidth > maxWidth) {
			maxWidth = wwidth;
		}
	}
	viewport()->setMinimumWidth(maxWidth);
}

void FlowScrollPane::resizeEvent(QResizeEvent* event) {
	int horPos = 0;
	int verPos = 0;
	int maxHeight = -1;
	int maxWidth = -1;
	// Do magic - make sure that magic involves margins between widgets AS WELL as margins between the widgets and the edge of the widget
	for(int j = 0; j < children.length(); j++) {
		int widgetWidth = children.at(j)->sizeHint().width() + margins.left() + margins.right();
		int widgetHeight = children.at(j)->sizeHint().height() + margins.top() + margins.bottom();

		int posY = horPos == 0 ? margins.left() : horPos;
		children.at(j)->setGeometry(posY, verPos + margins.top(), children.at(j)->sizeHint().width(), children.at(j)->sizeHint().height());
		horPos += widgetWidth;
		maxHeight = widgetHeight > maxHeight ? widgetHeight : maxHeight;

		if(horPos > viewport()->width()) {
			maxWidth = horPos > maxWidth ? horPos : maxWidth;
			verPos += maxHeight;
			horPos = 0;
		}
	}

// 	if(maxWidth > event->size().width()) {
// 		resize(maxWidth, event->size().height());
// 		QScrollArea::resizeEvent(event);
// 	}
}