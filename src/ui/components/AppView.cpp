#include "AppView.h"
#include <QTabBar>
#include <QGridLayout>
#include <QGraphicsView>
#include "ProjectView.h"
#include <QPushButton>
#include <QSizePolicy>

AppView::AppView(AppModel* model) : m_Model(model), QMainWindow() {
	QTabWidget* tabs = new QTabWidget();
	tabs->setTabsClosable(true);
	tabs->setMovable(true);
	tabs->tabBar()->setExpanding(true);
	tabs->tabBar()->setDocumentMode(true);

	connect(tabs, &QTabWidget::tabCloseRequested, this, &AppView::closeProject);

	this->m_Tabs = tabs;

	for(ProjectModel* project : *model->projects()) {
		this->addProject(project);
	}

	connect(m_Model, &AppModel::projectAdded, this, &AppView::addProject);

	QPushButton* newTabBtn = new QPushButton("New");

	connect(newTabBtn, &QPushButton::clicked, [this](bool checked) {
		this->m_Model->newProject(DEFAULT_CANVAS_WIDTH, DEFAULT_CANVAS_HEIGHT);
	});

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