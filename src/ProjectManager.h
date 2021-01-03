#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "Project.h"

class ProjectManager {
private:
	ProjectManager() = delete;

public:
	static QList<Project*> projects;
	static QHash<int, Project*> projectIds; // ids mapped to projects

	static void registerNewProject(Project* project);
	static void registerCloseProject(Project* project);
	static int getNewId();
	static Project* fromId(int id);
	static int id(Project* proj);
};

#endif // PROJECTMANAGER_H
