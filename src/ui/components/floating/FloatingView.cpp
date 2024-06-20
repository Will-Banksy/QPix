#include "FloatingView.h"
#include "model/AppModel.h"
#include "tooltip/FloatingTooltipView.h"
#include <iostream>
#include <QVBoxLayout>
#include "utils/Utils.h"

FloatingView::FloatingView(QWidget* parent, AppModel* model) : QWidget(parent), m_TooltipView(new FloatingTooltipView(this, model)) {
	this->setVisible(true);
	this->setAttribute(Qt::WA_TransparentForMouseEvents);

	connect(model, &AppModel::floatingInfoRequested, this, &FloatingView::showFloatingInfo);
	connect(model, &AppModel::hideFloating, this, &FloatingView::hideTooltips);

	connect(model, &AppModel::modalColourSelectRequested, this, &FloatingView::showColourSelectPopup);
}

FloatingView::~FloatingView() {
}

void FloatingView::showFloatingInfo(QWidget* src, const QString& title, const QString& body, FloatingPosition position) {
	m_TooltipView->showFloatingInfo(src, title, body, position);

	this->raise();
}

void FloatingView::hideTooltips() {
	m_TooltipView->hide();
}

void FloatingView::showColourSelectPopup(QWidget* src, const QColor& colour, std::function<void(const QColor&)> callback) {
	// We want to capture click events outside of the modal widget now, so we can dismiss the modal widget
	this->setAttribute(Qt::WA_TransparentForMouseEvents, false);

	std::cerr << "Showing colour select popup!" << std::endl;
}

void FloatingView::dismissPopup() {
	this->setAttribute(Qt::WA_TransparentForMouseEvents);
}

void FloatingView::mousePressEvent(QMouseEvent* event) {
	std::cout << "Detected mouse click (for dismissal purposes)" << std::endl;

	this->dismissPopup();
}