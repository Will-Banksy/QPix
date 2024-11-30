#ifndef TOOLSETTINGSVIEW_H
#define TOOLSETTINGSVIEW_H

#include <QWidget>
#include <QMetaObject>
#include <QList>

class AppModel;
class AbstractTool;
class QStackedLayout;

class ToolSettingsView : public QWidget {
	Q_OBJECT

public:
	explicit ToolSettingsView(AppModel* model);
	~ToolSettingsView() override;

private:
	AppModel* m_Model;
	QStackedLayout* m_Layout;
	QHash<AbstractTool*, int> m_IndexMap;

	void switchUi(AbstractTool* tool);
};

#endif // TOOLSETTINGSVIEW_H