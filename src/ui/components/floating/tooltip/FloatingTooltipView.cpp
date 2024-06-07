#include "FloatingTooltipView.h"
#include "model/AppModel.h"
#include <QStackedLayout>
#include "FloatingInfoView.h"
#include <iostream>
#include <QVBoxLayout>
#include "utils/Utils.h"

FloatingTooltipView::FloatingTooltipView(QWidget* parent, AppModel* model) : QWidget(parent), m_InfoView(new FloatingInfoView()) {
	this->setVisible(false);
	this->setAttribute(Qt::WA_StyledBackground);
	this->setAttribute(Qt::WA_TransparentForMouseEvents);
	this->setObjectName("floating");

	// this->setContentsMargins(4, 4, 4, 4); // Hard coding this for now

	QStackedLayout* layout = new QStackedLayout();

	layout->addWidget(m_InfoView);

	this->setLayout(layout);

	connect(model, &AppModel::floatingInfoRequested, this, &FloatingTooltipView::showFloatingInfo);
	connect(model, &AppModel::hideFloating, this, &FloatingTooltipView::hide);
}

FloatingTooltipView::~FloatingTooltipView() {
}

void FloatingTooltipView::showFloatingInfo(QWidget* src, const QString& title, const QString& body, FloatingPosition position) {
	std::cerr << "showFloatingInfo" << std::endl;

	if(this->window() != src->window()) { // Can't display on other windows
		return;
	}

	this->setVisible(true);

	m_InfoView->setTitle(title);
	m_InfoView->setBody(body);
	m_InfoView->updateGeometry();

	((QStackedLayout*)this->layout())->setCurrentWidget(m_InfoView);

	this->adjustSize();

	this->reposition(src, position);

	this->update();
}

void FloatingTooltipView::hide() {
	this->setVisible(false);
}

void FloatingTooltipView::reposition(QWidget* src, FloatingPosition position) {
	this->move(utils::repositionFloating(this, src, position));
}
