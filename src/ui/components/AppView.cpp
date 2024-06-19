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
#include "toolbars/ColourSelectView.h"
#include "floating/FloatingView.h"
#include "floating/tooltip/HoverInfoEventFilter.h"
#include <QResizeEvent>
#include <QDockWidget>

// BUG: Moving between tabs and resizing can centre other canvases sometimes, or otherwise unexpectedly change the scroll position of other projects

AppView::AppView(AppModel* model) : m_Model(model), QMainWindow() {
	this->setWindowTitle("QPix");
	this->setDockOptions(QMainWindow::DockOption::AllowNestedDocks | QMainWindow::DockOption::AllowTabbedDocks | QMainWindow::DockOption::AnimatedDocks | QMainWindow::DockOption::VerticalTabs);

	// Tab widget

	QTabWidget* tabs = new QTabWidget();
	tabs->setTabsClosable(true);
	tabs->setMovable(true);
	tabs->tabBar()->setExpanding(true);
	tabs->tabBar()->setDocumentMode(true);

	this->m_Tabs = tabs;

	connect(tabs, &QTabWidget::tabCloseRequested, this, &AppView::closeProject);
	connect(tabs, &QTabWidget::currentChanged, this, [this](int index) {
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
		newTabBtn,
		"New Project",
		"Creates a new default project",
		FloatingPosition::Bottom
	));

	connect(newTabBtn, &QPushButton::clicked, this, [this](bool checked) {
		this->m_Model->newProject(DEFAULT_CANVAS_WIDTH, DEFAULT_CANVAS_HEIGHT);
	});

	// Central widget

	QGridLayout* layout = new QGridLayout();
	layout->addWidget(newTabBtn, 0, 0);
	layout->addWidget(tabs, 1, 0);

	QWidget* central = new QWidget();
	central->setLayout(layout);
	this->setCentralWidget(central);

	// Toolbars (in the form of DockWidgets)

	QDockWidget* toolToolDock = new QDockWidget("Tools");
	toolToolDock->setFeatures(QDockWidget::DockWidgetFeature::DockWidgetMovable);

	ToolSelectView* toolToolbar = new ToolSelectView(model);
	toolToolDock->setWidget(toolToolbar);

	this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, toolToolDock);

	QDockWidget* colourToolDock = new QDockWidget("Colour");
	colourToolDock->setFeatures(QDockWidget::DockWidgetFeature::DockWidgetMovable);

	ColourSelectView* colourToolbar = new ColourSelectView(model);
	colourToolDock->setWidget(colourToolbar);

	this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, colourToolDock);

	// Status bar

	this->statusBar();
	this->statusBar()->addPermanentWidget(new StatusZoomView(model));

	// Floating widget

	this->m_Floating = new FloatingView(this, model);
}

AppView::~AppView() {
}

void AppView::resizeEvent(QResizeEvent* event) {
	m_Floating->setGeometry(0, 0, event->size().width(), event->size().height());
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
