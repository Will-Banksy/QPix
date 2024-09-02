#include "FloatingModalView.h"
#include <QStackedLayout>
#include "utils/Utils.h"
#include "model/AppModel.h"
#include <QKeyEvent>
#include "ui/widgets/ColourSelector.h"

FloatingModalView::FloatingModalView(QWidget* parent, AppModel* model) : QWidget(parent), m_Model(model), m_ColourSelector(new ColourSelector(QColorConstants::Black)), m_ColourSelectConnection(nullptr) {
	this->setVisible(false);
	this->setAttribute(Qt::WA_StyledBackground);
	this->setObjectName("floating");

	this->setMinimumSize(64, 64);

	QStackedLayout* layout = new QStackedLayout();

	layout->addWidget(m_ColourSelector);

	this->setLayout(layout);
}

FloatingModalView::~FloatingModalView() {
}

void FloatingModalView::showColourSelectPopup(QWidget* src, const QColor& colour, ColourChangeCallback callback, FloatingPosition position) {
	this->setVisible(true);

	m_ColourSelector->setColour(colour);

	// Make a temporary connection so that the callback is called on colour change, storing the connection handle
	m_ColourSelectConnection = new QMetaObject::Connection(
		connect(m_ColourSelector, &ColourSelector::colourChanged, [callback](const QColor& colour) {
			callback(colour);
		})
	);

	((QStackedLayout*)this->layout())->setCurrentWidget(m_ColourSelector);

	m_ColourSelector->updateGeometry();

	this->updateGeometry();
	this->move(utils::repositionFloating(this, src, position));
	this->update();
}

void FloatingModalView::hide() {
	this->setVisible(false);

	// Delete the temporary connection if there is one
	if(m_ColourSelectConnection != nullptr) {
		m_ColourSelector->disconnect(*m_ColourSelectConnection);
		m_ColourSelectConnection = nullptr;
	}
}

void FloatingModalView::keyPressEvent(QKeyEvent* event) {
	if(event->matches(QKeySequence::Cancel)) {
		emit m_Model->hidePopups();
	}
}
