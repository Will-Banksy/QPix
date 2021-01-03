#include "ProjectManager.h"
#include "ui/Window.h"
#include "ui/TabbedProjectView.h"
#include <iostream>

QList<Project*> ProjectManager::projects;
QHash<int, Project*> ProjectManager::projectIds;

void ProjectManager::registerNewProject(Project* project) {
	project->id = getNewId();
	std::cout << "ProjectId: " << project->id << std::endl;
	projects.append(project);
	projectIds.insert(project->id, project);
}

void ProjectManager::registerCloseProject(Project* project) {
	projects.removeAll(project);
	projectIds.remove(project->id);
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
