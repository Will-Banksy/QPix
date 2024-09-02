#include "FloatingView.h"
#include "model/AppModel.h"
#include "tooltip/FloatingTooltipView.h"
#include <iostream>
#include <QVBoxLayout>
#include "utils/Utils.h"
#include "modal/FloatingModalView.h"
#include <QMouseEvent>

FloatingView::FloatingView(QWidget* parent, AppModel* model) : QWidget(parent), m_TooltipView(new FloatingTooltipView(this, model)), m_ModalView(new FloatingModalView(this, model)) {
	this->setVisible(true);
	this->setAttribute(Qt::WA_TransparentForMouseEvents);

	connect(model, &AppModel::floatingInfoRequested, this, &FloatingView::showFloatingInfo);
	connect(model, &AppModel::hideFloating, this, &FloatingView::hideTooltips);

	connect(model, &AppModel::modalColourSelectRequested, this, &FloatingView::showColourSelectPopup);
	connect(model, &AppModel::hidePopups, this, &FloatingView::dismissPopup);
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

void FloatingView::showColourSelectPopup(QWidget* src, const QColor& colour, ColourChangeCallback callback, FloatingPosition position) {
	// We want to capture click events outside of the modal widget now, so we can dismiss the modal widget
	this->setAttribute(Qt::WA_TransparentForMouseEvents, false);
	m_ModalView->showColourSelectPopup(src, colour, callback, position);
	m_ModalView->setFocus();

	this->raise();
}

void FloatingView::dismissPopup() {
	this->setAttribute(Qt::WA_TransparentForMouseEvents);

	m_ModalView->hide();
}

void FloatingView::mousePressEvent(QMouseEvent* event) {
	if(!m_ModalView->geometry().contains(event->position().toPoint())) {
		this->dismissPopup();
	}
}