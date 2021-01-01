#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "Project.h"

class ProjectManager {
private:
	ProjectManager() = delete;

public:
	static QList<Project*> projects;
	static QHash<int, Project*> projectIds; // ids mapped to projects

	static Project* createProject(Window* window);
	static void closeProject(Project* project);
	static int getNewId();
	static Project* fromId(int id);
	static int id(Project* proj);
};

#endif // PROJECTMANAGER_H
