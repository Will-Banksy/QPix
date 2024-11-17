#ifndef TOOLSETTINGSVIEW_H
#define TOOLSETTINGSVIEW_H

#include <QWidget>
#include <QMetaObject>
#include <QList>

class AppModel;
class AbstractTool;

class ToolSettingsView : public QWidget {
	Q_OBJECT

public:
	explicit ToolSettingsView(AppModel* model);
	~ToolSettingsView() override;

private:
	AppModel* m_Model;
	QList<QMetaObject::Connection> m_Connections;

	void regenUi(AbstractTool* tool);
};

#endif // TOOLSETTINGSVIEW_H