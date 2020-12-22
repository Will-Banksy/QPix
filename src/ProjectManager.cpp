#include "ProjectManager.h"

QList<Project*> ProjectManager::projects;

Project* ProjectManager::createProject(Window* window) {
	Project* project = new Project(window);
	projects.append(project);
	return project;
}

void ProjectManager::closeProject(Project* project) {
	projects.removeAll(project);
}
