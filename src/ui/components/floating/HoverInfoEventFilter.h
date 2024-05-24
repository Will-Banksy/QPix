#ifndef HOVERINFOEVENTFILTER_H
#define HOVERINFOEVENTFILTER_H

#include <QObject>

class QEvent;
class AppModel;

class HoverInfoEventFilter : public QObject {
	Q_OBJECT

public:
	explicit HoverInfoEventFilter(AppModel* model, QString title, QString body);
	~HoverInfoEventFilter();

protected:
	bool eventFilter(QObject* obj, QEvent* event);

private:
	AppModel* m_Model;
	QString m_Title;
	QString m_Body;
};

#endif // HOVERINFOEVENTFILTER_H