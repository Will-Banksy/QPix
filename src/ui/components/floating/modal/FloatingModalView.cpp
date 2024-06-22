#include "FloatingModalView.h"
#include <QStackedLayout>
#include "utils/Utils.h"
#include "model/AppModel.h"
#include <QKeyEvent>

FloatingModalView::FloatingModalView(QWidget* parent, AppModel* model) : QWidget(parent), m_Model(model) {
	this->setVisible(false);
	this->setAttribute(Qt::WA_StyledBackground);
	this->setObjectName("floating");

	this->setMinimumSize(64, 64);

	QStackedLayout* layout = new QStackedLayout();

	// TODO: Implement this like the FloatingTooltipView class

	this->setLayout(layout);
}

FloatingModalView::~FloatingModalView() {
}

void FloatingModalView::showColourSelectPopup(QWidget* src, const QColor& colour, ColourChangeCallback callback, FloatingPosition position) {
	this->setVisible(true);

	this->move(utils::repositionFloating(this, src, position));
	this->update();
}

void FloatingModalView::hide() {
	this->setVisible(false);
}

void FloatingModalView::keyPressEvent(QKeyEvent* event) {
	if(event->matches(QKeySequence::Cancel)) {
		emit m_Model->hidePopups();
	}
}
