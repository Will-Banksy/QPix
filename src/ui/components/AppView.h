#ifndef APPVIEW_H
#define APPVIEW_H

#include <QMainWindow>
#include "model/AppModel.h"

class AppView : public QMainWindow {
	Q_OBJECT

public:
	explicit AppView(AppModel* model);
	~AppView() override;

protected:
	void resizeEvent(QResizeEvent* event) override;
	void closeEvent(QCloseEvent* event) override;

private:
	AppModel* m_Model;

	QTabWidget* m_Tabs;
	FloatingView* m_Floating;

	QList<QAction*> m_ToggleViewActions;
	QList<QAction*> m_ProjectDependentActions;

	void createDocks();
	void createMenus();
	void createStatusBar();

	i32 tabIdxFor(const ProjectModel* project);

private slots:
	void addProject(ProjectModel* project);
	void closeProject(int index);
};

#endif // APPVIEW_H