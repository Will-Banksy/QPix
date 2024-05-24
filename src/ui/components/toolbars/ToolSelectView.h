#ifndef TOOLSELECTVIEW_H
#define TOOLSELECTVIEW_H

#include <QToolBar>

class AppModel;

class ToolSelectView : public QToolBar {
	Q_OBJECT

public:
	explicit ToolSelectView(AppModel* model);
	~ToolSelectView() override;

private:
	QActionGroup* m_ActionGroup;
};

#endif // TOOLSELECTVIEW_H