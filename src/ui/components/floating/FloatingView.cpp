#include "FloatingView.h"
#include "model/AppModel.h"
#include <QStackedLayout>
#include "tooltip/FloatingTooltipView.h"
#include <iostream>
#include <QVBoxLayout>
#include "utils/Utils.h"

FloatingView::FloatingView(QWidget* parent, AppModel* model) : QWidget(parent), m_TooltipView(new FloatingTooltipView(this, model)) {
	this->setVisible(true);
	this->setAttribute(Qt::WA_TransparentForMouseEvents);

	connect(model, &AppModel::floatingInfoRequested, this, &FloatingView::showFloatingInfo); // NOTE: Could I simply instead connect this directly to the FloatingTooltipView::showFloatingInfo?
	connect(model, &AppModel::hideFloating, this, &FloatingView::hideTooltips);
}

FloatingView::~FloatingView() {
}

void FloatingView::showFloatingInfo(QWidget* src, const QString& title, const QString& body, FloatingPosition position) {
	m_TooltipView->showFloatingInfo(src, title, body, position);
}

void FloatingView::hideTooltips() {
	m_TooltipView->hide();
}