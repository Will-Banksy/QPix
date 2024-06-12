#ifndef FLOATINGMODALVIEW_H
#define FLOATINGMODALVIEW_H

#include <QWidget>

class AppModel;

class FloatingModalView : public QWidget {
	Q_OBJECT

public:
	explicit FloatingModalView(QWidget* parent, AppModel* model);
	~FloatingModalView() override;
};

#endif // FLOATINGMODALVIEW_H