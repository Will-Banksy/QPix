#include "HoverInfoEventFilter.h"
#include "model/AppModel.h"

HoverInfoEventFilter::HoverInfoEventFilter(AppModel* model, QString title, QString body) : QObject(), m_Model(model), m_Title(title), m_Body(body) {
}

HoverInfoEventFilter::~HoverInfoEventFilter() {
}

bool HoverInfoEventFilter::eventFilter(QObject* obj, QEvent* event) {
	if(event->type() == QEvent::Enter) {
		emit m_Model->floatingInfoRequested((QWidget*)obj, m_Title, m_Body);
	} else if(event->type() == QEvent::Leave) {
		emit m_Model->hideFloating();
	}

	return false;
}
