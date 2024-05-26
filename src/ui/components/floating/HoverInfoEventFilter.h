#ifndef HOVERINFOEVENTFILTER_H
#define HOVERINFOEVENTFILTER_H

#include <QObject>
#include "FloatingView.h"

class QEvent;
class AppModel;
class QWidget;

class HoverInfoEventFilter : public QObject {
	Q_OBJECT

public:
	explicit HoverInfoEventFilter(AppModel* model, QWidget* widget, QString title, QString body, FloatingPosition position = FloatingPosition::Unspecified);
	~HoverInfoEventFilter();

protected:
	bool eventFilter(QObject* obj, QEvent* event);

private:
	AppModel* m_Model;
	QWidget* m_Widget;
	QString m_Title;
	QString m_Body;
	FloatingPosition m_Position;
};

#endif // HOVERINFOEVENTFILTER_H