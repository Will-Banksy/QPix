#include "AppView.h"
#include <QTabBar>
#include <QGridLayout>
#include <QGraphicsView>
#include "ProjectView.h"
#include <iostream>
#include <QPushButton>
#include <QSizePolicy>

AppView::AppView(AppModel* model) : m_Model(model), QMainWindow() {
	QTabWidget* tabs = new QTabWidget();
	tabs->setTabsClosable(true);
	tabs->setMovable(true);
	tabs->tabBar()->setExpanding(true);
	tabs->tabBar()->setDocumentMode(true);

	connect(tabs, &QTabWidget::tabCloseRequested, this, &AppView::closeProject);

	std::cerr << "AppView" << std::endl;

	this->m_Tabs = tabs;

	for(ProjectModel* project : *model->projects()) {
		this->addProject(project);
	}

	connect(m_Model, &AppModel::projectAdded, this, &AppView::addProject);

	QPushButton* newTabBtn = new QPushButton("New");

	connect(newTabBtn, &QPushButton::clicked, m_Model, &AppModel::newProject);

	QGridLayout* layout = new QGridLayout();
	layout->addWidget(newTabBtn, 0, 0);
	layout->addWidget(tabs, 1, 0);

	QWidget* central = new QWidget();
	central->setLayout(layout);
	this->setCentralWidget(central);
}

AppView::~AppView() {
}

void AppView::addProject(ProjectModel* project) {
	ProjectView* view = new ProjectView(project, this->m_Model);

	this->m_Tabs->addTab(view, project->path());
}

void AppView::closeProject(int index) {
	ProjectView* projectView = (ProjectView*)this->m_Tabs->widget(index);
	ProjectModel* project = projectView->model();
	this->m_Model->closeProject(project);

	this->m_Tabs->removeTab(index);
	delete projectView;
}