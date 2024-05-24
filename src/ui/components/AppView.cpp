#include "AppView.h"
#include <QTabBar>
#include <QGridLayout>
#include <QGraphicsView>
#include "ProjectView.h"
#include <QPushButton>
#include <QSizePolicy>
#include <QStatusBar>
#include "status/StatusZoomView.h"
#include "toolbars/ToolSelectView.h"
#include "floating/FloatingView.h"
#include "floating/HoverInfoEventFilter.h"

AppView::AppView(AppModel* model) : m_Model(model), QMainWindow() {
	// Tab widget

	QTabWidget* tabs = new QTabWidget();
	tabs->setTabsClosable(true);
	tabs->setMovable(true);
	tabs->tabBar()->setExpanding(true);
	tabs->tabBar()->setDocumentMode(true);

	this->m_Tabs = tabs;

	connect(tabs, &QTabWidget::tabCloseRequested, this, &AppView::closeProject);
	connect(tabs, &QTabWidget::currentChanged, [this](int index) {
		ProjectView* view = (ProjectView*)this->m_Tabs->widget(index);
		if(view != nullptr) {
			ProjectModel* project = view->model();
			this->m_Model->updateCurrProject(Nullable(project));
		} else {
			this->m_Model->updateCurrProject(Nullable<ProjectModel>());
		}
	});

    for(ProjectModel* project : model->projects()) {
		this->addProject(project);
	}

	connect(m_Model, &AppModel::projectAdded, this, &AppView::addProject);

	// New tab button

	QPushButton* newTabBtn = new QPushButton("New");
	newTabBtn->installEventFilter(new HoverInfoEventFilter(
		m_Model,
		"New Project",
		"Creates a new default project"
	));

	connect(newTabBtn, &QPushButton::clicked, [this](bool checked) {
		this->m_Model->newProject(DEFAULT_CANVAS_WIDTH, DEFAULT_CANVAS_HEIGHT);
	});

	// Central widget

	QGridLayout* layout = new QGridLayout();
	layout->addWidget(newTabBtn, 0, 0);
	layout->addWidget(tabs, 1, 0);

	QWidget* central = new QWidget();
	central->setLayout(layout);
	this->setCentralWidget(central);

	// Toolbars

	ToolSelectView* toolToolbar = new ToolSelectView(m_Model);
	this->addToolBar(Qt::ToolBarArea::LeftToolBarArea, toolToolbar);

	// Status bar

	this->statusBar();
	this->statusBar()->addPermanentWidget(new StatusZoomView(model));

	// Floating widget

	FloatingView* floating = new FloatingView(this, model);
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
	projectView->deleteLater();
}
