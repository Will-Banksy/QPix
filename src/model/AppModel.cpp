#include "AppModel.h"

AppModel::AppModel() : QObject() {
}

AppModel::~AppModel() {
}

QList<ProjectModel*>* AppModel::projects() {
	return &this->m_Projects;
}

void AppModel::newProject() {
	ProjectModel* newProject = new ProjectModel();
	this->m_Projects.append(newProject);
	emit projectAdded(newProject);
}

void AppModel::closeProject(ProjectModel* project) {
	for(int i = 0; i < this->m_Projects.size(); i++) {
		if(this->m_Projects[i] == project) {
			m_Projects.removeAt(i);
			delete project;
		}
	}
}