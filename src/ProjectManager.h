#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "Project.h"

class ProjectManager {
private:
	ProjectManager() = delete;

public:
	static QList<Project*> projects;

	static Project* createProject(Window* window);
	static void closeProject(Project* project);
};

#endif // PROJECTMANAGER_H
