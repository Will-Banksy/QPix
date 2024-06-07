#include "HoverInfoEventFilter.h"
#include "model/AppModel.h"
#include <QWidget>

HoverInfoEventFilter::HoverInfoEventFilter(AppModel* model, QWidget* widget, QString title, QString body, FloatingPosition position) : QObject(), m_Widget(widget), m_Model(model), m_Title(title), m_Body(body), m_Position(position) {
}

HoverInfoEventFilter::~HoverInfoEventFilter() {
}

bool HoverInfoEventFilter::eventFilter(QObject* obj, QEvent* event) {
	if(event->type() == QEvent::Enter) {
		emit m_Model->floatingInfoRequested(m_Widget, m_Title, m_Body, m_Position);
	} else if(event->type() == QEvent::Leave) {
		emit m_Model->hideFloating();
	} else if(event->type() == QEvent::ToolTip) {
		return true; // Stop the widget from recieving tooltip events
	}

	return false;
}
