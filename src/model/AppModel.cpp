#include "AppModel.h"

AppModel::AppModel() : QObject(),
	m_Projects(QList<ProjectModel*>()),
	m_CurrProject(Nullable<ProjectModel>()),
	m_AvailableTools(Tool::initialiseTools()),
	m_CurrentTool(m_AvailableTools[0]),
	m_PrimaryColour(QColorConstants::Black),
	m_SecondaryColour(QColorConstants::Transparent) {
}

AppModel::~AppModel() {
	for(ProjectModel* model : m_Projects) {
		model->deleteLater();
	}
}

QList<ProjectModel*>* AppModel::projects() {
	return &this->m_Projects;
}

Nullable<ProjectModel> AppModel::currProject() {
	return Nullable(m_CurrProject);
}

QColor AppModel::primaryColour() {
	return m_PrimaryColour;
}

QColor AppModel::secondaryColour() {
	return m_SecondaryColour;
}

void AppModel::newProject(int width, int height) {
	ProjectModel* newProject = new ProjectModel();
	this->m_Projects.append(newProject);
	emit projectAdded(newProject);
	connect(newProject, &ProjectModel::anythingUpdated, [this, newProject]() {
		emit updateCurrProject(Nullable(newProject));
	});
}

void AppModel::openProject(QString& path) {
	ProjectModel* newProject = new ProjectModel(path);
	this->m_Projects.append(newProject);
	emit projectAdded(newProject);
	connect(newProject, &ProjectModel::anythingUpdated, [this, newProject]() {
		emit updateCurrProject(Nullable(newProject));
	});
}

void AppModel::closeProject(ProjectModel* project) {
	for(int i = 0; i < this->m_Projects.size(); i++) {
		if(this->m_Projects[i] == project) {
			m_Projects.removeAt(i);
			project->deleteLater();
		}
	}
}

void AppModel::updateCurrProject(Nullable<ProjectModel> project) { // NOTE: m_CurrProject is now set before emitting currProjectUpdated - Desired behaviour?
	m_CurrProject = project;
	emit currProjectUpdated(project);
}