#ifndef TABBEDPROJECTVIEW_H
#define TABBEDPROJECTVIEW_H

#include <QWidget>
#include "Project.h"

class CanvasView;
class QTabBar;

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
	void setCurrentProject(int tabIndex);
	void handleTabClose(int tabIndex);

// 	QList<Project*> projects;
// 	QTabBar* tabbar;
// 	CanvasView* view;
// 	QHash<int, Project*> projectTabIds;
//
// 	void addProject(Project* project);
// 	void closeProject(Project* project);
// 	Project* fromId(int id);
// 	int id(Project* project);
// 	void addId(int id, Project* project);
// 	void removeId(int id);

// public slots:
// 	void setCurrentProject(int index);
// 	void handleTabClose(int index);
};

#endif // TABBEDPROJECTVIEW_H
