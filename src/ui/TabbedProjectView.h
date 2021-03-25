#ifndef TABBEDPROJECTVIEW_H
#define TABBEDPROJECTVIEW_H

#include <QWidget>
#include "Project.h"

class CanvasView;
class QTabBar;
class UI;

class TabbedProjectView : public QWidget {
	Q_OBJECT

public:
	TabbedProjectView(CanvasView* view, QList<Project*> projects, QWidget* parent = nullptr);
	virtual ~TabbedProjectView() = default;

	QTabBar* tabbar;
	CanvasView* view;

	void registerNewProject(Project* project);
	void registerCloseProject(Project* project, int tabIndex = -1);
	QList<Project*> getProjects();
	void closeAllProjects();

public slots:
	void setCurrentProject(Project* project); // So far unused
	void setCurrentProject(int tabIndex);
	void handleTabClose(int tabIndex);
};

#endif // TABBEDPROJECTVIEW_H
