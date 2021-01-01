#include "ProjectManager.h"
#include "Window.h"
#include "ui/TabbedProjectView.h"

QList<Project*> ProjectManager::projects;
QHash<int, Project*> ProjectManager::projectIds;

Project* ProjectManager::createProject(Window* window) {
	Project* project = new Project(window, getNewId());
	projects.append(project);
	window->ui->tabbedView->addProject(project);
	projectIds.insert(project->id, project);
	return project;
}

void ProjectManager::closeProject(Project* project) {
	projects.removeAll(project);
	projectIds.remove(project->id);
// 	delete project;
}

int ProjectManager::getNewId() {
	for(int i = 0;; i++) {
		bool hasI = false;
		for(int j = 0; j < projects.count(); j++) {
			if(projects.at(j)->id == i) {
				hasI = true;
				break;
			}
		}
		if(!hasI) {
			return i;
		}
	}
}

Project* ProjectManager::fromId(int id) {
	return projectIds.value(id); // Returns nullptr if not found
}

int ProjectManager::id(Project* proj) {
	return proj->id;
}
