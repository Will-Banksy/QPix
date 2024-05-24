#include "AppModel.h"

AppModel::AppModel() : QObject(),
	m_Projects(QList<ProjectModel*>()),
	m_CurrProject(Nullable<ProjectModel>()),
    m_AvailableTools(AbstractTool::initialiseTools()),
	m_CurrentTool(m_AvailableTools.at(0)),
	m_PrimaryColour(QColorConstants::Black),
	m_SecondaryColour(QColorConstants::Transparent) {
}

AppModel::~AppModel() {
	for(ProjectModel* model : m_Projects) {
		model->deleteLater();
	}
	for(AbstractTool* tool : m_AvailableTools) {
		tool->deleteLater();
	}
}

const QList<ProjectModel*>& AppModel::projects() const {
    return this->m_Projects;
}

Nullable<ProjectModel> AppModel::currProject() const {
	return Nullable(m_CurrProject);
}

const QList<AbstractTool*>& AppModel::availableTools() const {
    return m_AvailableTools;
}

AbstractTool* AppModel::currentTool() const {
    return m_CurrentTool;
}

QColor AppModel::primaryColour() const {
	return m_PrimaryColour;
}

QColor AppModel::secondaryColour() const {
	return m_SecondaryColour;
}

void AppModel::newProject(int width, int height) {
	ProjectModel* newProject = new ProjectModel(width, height);
	this->m_Projects.append(newProject);
	emit projectAdded(newProject);
	connect(newProject, &ProjectModel::anythingUpdated, [this, newProject]() {
        updateCurrProject(Nullable(newProject));
	});
}

void AppModel::openProject(QString& path) {
	ProjectModel* newProject = new ProjectModel(path);
	this->m_Projects.append(newProject);
	emit projectAdded(newProject);
	connect(newProject, &ProjectModel::anythingUpdated, [this, newProject]() {
        updateCurrProject(Nullable(newProject));
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

void AppModel::changeTool(AbstractTool* tool) {
	m_CurrentTool = tool;
	emit toolChanged(tool);
}
