#include "FloatingView.h"
#include "model/AppModel.h"
#include <QStackedLayout>
#include "FloatingInfoView.h"
#include <iostream>
#include <QVBoxLayout>

// TODO: How am I gonna implement this...? Probably just a widget of each supported type in a QStackedLayout that I switch between and update with the necessary information right?

FloatingView::FloatingView(QWidget* parent, AppModel* model) : QWidget(parent), m_InfoView(new FloatingInfoView()) {
	this->setAutoFillBackground(true);
	this->setVisible(false);

	m_InfoView->setContentsMargins(8, 8, 8, 8);

	QStackedLayout* layout = new QStackedLayout();

	layout->addWidget(m_InfoView);

	this->setLayout(layout);

	connect(model, &AppModel::floatingInfoRequested, this, &FloatingView::showFloatingInfo);
	connect(model, &AppModel::hideFloating, this, &FloatingView::hide);
}

FloatingView::~FloatingView() {
}

void FloatingView::showFloatingInfo(QWidget* src, const QString& title, const QString& body) {
	std::cerr << "showFloatingInfo" << std::endl;

	this->setVisible(true);

	m_InfoView->setTitle(title);
	m_InfoView->setBody(body);
	m_InfoView->updateGeometry();

	((QStackedLayout*)this->layout())->setCurrentWidget(m_InfoView);

	QPointF srcWindowCoords = src->mapTo(src->window(), src->pos());

	this->move(srcWindowCoords.x(), srcWindowCoords.y() + 50);

	// this->resize(m_InfoView->sizeHint());
	this->adjustSize();
	this->update();
}

void FloatingView::showColourSelect(QWidget* src, QColor current) {
}

void FloatingView::hide() {
	this->setVisible(false);
}
