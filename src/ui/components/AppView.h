#ifndef APPVIEW_H
#define APPVIEW_H

#include <QMainWindow>
#include "model/AppModel.h"

class AppView : public QMainWindow {
	Q_OBJECT

public:
	explicit AppView(AppModel* model);
	~AppView() override;

private:
	AppModel* m_Model;
	QTabWidget* m_Tabs;

private slots:
	void addProject(ProjectModel* project);
	void closeProject(int index);
};

#endif // APPVIEW_H