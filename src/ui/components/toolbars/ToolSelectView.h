#ifndef TOOLSELECTVIEW_H
#define TOOLSELECTVIEW_H

#include <QWidget>

class AppModel;

class ToolSelectView : public QWidget {
	Q_OBJECT

public:
	explicit ToolSelectView(AppModel* model);
	~ToolSelectView() override;
};

#endif // TOOLSELECTVIEW_H