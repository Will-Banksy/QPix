#ifndef FLOATINGINFOVIEW_H
#define FLOATINGINFOVIEW_H

#include <QWidget>

class QLabel;

class FloatingInfoView : public QWidget {
	Q_OBJECT

public:
	explicit FloatingInfoView();
	~FloatingInfoView() override;

	void setTitle(const QString& text);
	void setBody(const QString& text);

private:
	QLabel* m_Title;
	QLabel* m_Body;
};

#endif // FLOATINGINFOVIEW_H