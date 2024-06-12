#include "FloatingModalView.h"
#include <QStackedLayout>

FloatingModalView::FloatingModalView(QWidget* parent, AppModel* model) {
	this->setVisible(false);
	this->setAttribute(Qt::WA_StyledBackground);
	this->setObjectName("floating");

	QStackedLayout* layout = new QStackedLayout();

	// TODO: Implement this like the FloatingTooltipView class

	this->setLayout(layout);
}

FloatingModalView::~FloatingModalView() {
}
