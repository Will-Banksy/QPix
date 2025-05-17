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
#include "toolbars/ToolSettingsView.h"
#include <QMenuBar>
#include <QFileDialog>
#include <QApplication>
#include <QMessageBox>
#include <QCloseEvent>
#include <QListWidget>
#include <QDialogButtonBox>
#include <QLabel>

// BUG: Moving between tabs and resizing can centre other canvases sometimes, or otherwise unexpectedly change the scroll position of other projects

AppView::AppView(AppModel* model) : QMainWindow(), m_Model(model), m_ToggleViewActions(), m_ProjectDependentActions() {
	this->setWindowTitle("QPix");
	this->setDockOptions(QMainWindow::DockOption::AllowNestedDocks | QMainWindow::DockOption::AllowTabbedDocks | QMainWindow::DockOption::AnimatedDocks | QMainWindow::DockOption::VerticalTabs);

	// Tab widget

	m_Tabs = new QTabWidget();
	m_Tabs->setTabsClosable(true);
	m_Tabs->setMovable(true);
	m_Tabs->tabBar()->setExpanding(true);
	m_Tabs->tabBar()->setDocumentMode(true);

	this->connect(m_Tabs, &QTabWidget::tabCloseRequested, this, &AppView::closeProject);
	this->connect(m_Tabs, &QTabWidget::currentChanged, this, [this](int index) {
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

	this->connect(m_Model, &AppModel::projectAdded, this, &AppView::addProject);

	this->connect(m_Model, &AppModel::currProjectUpdated, this, [this](Nullable<ProjectModel> projOpt) {
		if(projOpt.isNotNull()) {
			ProjectModel* proj = projOpt.unwrap();
			i32 projTabIdx = this->tabIdxFor(proj);
			if(projTabIdx != this->m_Tabs->currentIndex()) {
				this->m_Tabs->setCurrentIndex(projTabIdx);
			}
		}
		this->statusBar()->clearMessage(); // NOTE: This is maybe a bit hasty. We don't necessarily want to clear the message on *any* event... Mainly not zoom tbh
	});

	// Central widget

	QGridLayout* layout = new QGridLayout();
	layout->addWidget(m_Tabs, 0, 0);

	QWidget* central = new QWidget();
	central->setLayout(layout);
	this->setCentralWidget(central);

	// Add dock widgets (toolbars)
	this->createDocks();

	// Add status bar
	this->createStatusBar();

	// Add menus
	this->createMenus();

	// Floating widget
	m_Floating = new FloatingView(this, model);
}

AppView::~AppView() {
}

void AppView::resizeEvent(QResizeEvent* event) {
	m_Floating->setGeometry(0, 0, event->size().width(), event->size().height());
}

void AppView::closeEvent(QCloseEvent* event) {
	// TODO: Handle multiple unsaved projects

	QList<QString> unsavedProjNames;
	for(const ProjectModel* proj : m_Model->projects()) {
		if(!proj->saved()) {
			unsavedProjNames.push_back(proj->displayName());
		}
	}

	if(!unsavedProjNames.isEmpty()) {
		QDialog* diag = new QDialog(this);
		diag->setWindowTitle("Discard changes?");

		QListWidget* projList = new QListWidget();
		for(const QString& name : unsavedProjNames) {
			projList->addItem(name);
		}

		QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::StandardButton::Cancel | QDialogButtonBox::Discard);

		diag->connect(buttons->button(QDialogButtonBox::StandardButton::Discard), &QPushButton::clicked, diag, &QDialog::accept);
		diag->connect(buttons, &QDialogButtonBox::rejected, diag, &QDialog::reject);

		QVBoxLayout* diagLayout = new QVBoxLayout();
		diagLayout->addWidget(new QLabel("These open projects have unsaved changes. Discard all changes and close QPix?"));
		diagLayout->addWidget(projList);
		diagLayout->addWidget(buttons);
		diag->setLayout(diagLayout);

		QDialog::DialogCode result = (QDialog::DialogCode)diag->exec();

		if(result == QDialog::DialogCode::Rejected) {
			event->ignore();
			return;
		}
	}

	QMainWindow::closeEvent(event);
}

void AppView::createDocks() {
	// Toolbars (in the form of DockWidgets)

	// TODO: Sort out the default sizes of docks to ensure their title is visible but they are not too big

	const QDockWidget::DockWidgetFeatures TOOL_DOCK_FEATURES = QDockWidget::DockWidgetFeature::DockWidgetClosable | QDockWidget::DockWidgetFeature::DockWidgetMovable;

	QDockWidget* toolToolDock = new QDockWidget("Tools");
	toolToolDock->setFeatures(TOOL_DOCK_FEATURES);

	ToolSelectView* toolToolbar = new ToolSelectView(m_Model);
	toolToolDock->setWidget(toolToolbar);

	this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, toolToolDock);
	m_ToggleViewActions.append(toolToolDock->toggleViewAction());

	QDockWidget* colourToolDock = new QDockWidget("Colour");
	colourToolDock->setFeatures(TOOL_DOCK_FEATURES);

	ColourSelectView* colourToolbar = new ColourSelectView(m_Model);
	colourToolDock->setWidget(colourToolbar);

	this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, colourToolDock);
	m_ToggleViewActions.append(colourToolDock->toggleViewAction());

	QDockWidget* toolSettingsToolDock = new QDockWidget("Tool Settings");
	toolSettingsToolDock->setFeatures(TOOL_DOCK_FEATURES);

	ToolSettingsView* toolSettingsToolbar = new ToolSettingsView(m_Model);
	toolSettingsToolDock->setWidget(toolSettingsToolbar);

	this->addDockWidget(Qt::DockWidgetArea::TopDockWidgetArea, toolSettingsToolDock);
	m_ToggleViewActions.append(toolSettingsToolDock->toggleViewAction());
}

void AppView::createMenus() {
	QMenuBar* menuBar = new QMenuBar();

	QMenu* file = new QMenu("&File");
	menuBar->addMenu(file);

	QAction* fileNew = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew), "&New");
	this->connect(fileNew, &QAction::triggered, this, [this]() {
		this->m_Model->newProject(DEFAULT_CANVAS_WIDTH, DEFAULT_CANVAS_HEIGHT);
	});
	fileNew->setShortcut(QKeySequence::StandardKey::New);

	QAction* fileOpen = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen), "&Open");
	this->connect(fileOpen, &QAction::triggered, this, [this]() {
		QList<QString> files = QFileDialog::getOpenFileNames(this, "Open image files", "", "Image files (*.png)");
		for(const auto& path : files) {
			const ProjectModel* alreadyOpenProj = nullptr;
			for(const ProjectModel* proj : this->m_Model->projects()) {
				if(proj->path() == path) {
					alreadyOpenProj = proj;
					break;
				}
			}
			if(alreadyOpenProj == nullptr) {
				this->m_Model->openProject(path);
			} else {
				i32 idx = this->tabIdxFor(alreadyOpenProj);
				this->m_Tabs->setCurrentIndex(idx);
			}
		}
	});
	fileOpen->setShortcut(QKeySequence::StandardKey::Open);

	QAction* fileSaveAs = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSaveAs), "Save &As...");
	this->connect(fileSaveAs, &QAction::triggered, this, [this]() {
		Nullable<ProjectModel> projOpt = this->m_Model->currProject();
		if(projOpt.isNotNull()) {
			ProjectModel* proj = projOpt.unwrap();
			QString savePath = QFileDialog::getSaveFileName(this, "Save project as...", "", "Image files (*.png)");
			if(!savePath.isEmpty()) {
				if(!savePath.endsWith(".png")) {
					savePath.append(".png");
				}
				QString prevPath = proj->path();
				proj->setPath(savePath);
				qDebug() << "Checkpoint (after setPath)";
				if(proj->save()) {
					this->statusBar()->showMessage(QString("Saved project to: %1").arg(savePath));
				} else {
					QMessageBox::critical(this, "Failed to save file", "Error: Failed to save project file.\nPlease check that you have the necessary permissions to write to this location, and that the path is not malformed.");
					proj->setPath(prevPath);
				}
			}
		}
	});
	fileSaveAs->setShortcut(QKeySequence::StandardKey::SaveAs);

	QAction* fileSave = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSave), "&Save");
	this->connect(fileSave, &QAction::triggered, this, [this, fileSaveAs]() {
		Nullable<ProjectModel> projOpt = this->m_Model->currProject();
		if(projOpt.isNotNull()) {
			ProjectModel* proj = projOpt.unwrap();
			if(!proj->hasPath()) {
				fileSaveAs->trigger();
			} else if(proj->save()) {
				this->statusBar()->showMessage(QString("Saved project to: %1").arg(proj->path()));
			} else {
				QMessageBox::critical(this, "Failed to save file", "Error: Failed to save project file.\nPlease check that you have the necessary permissions to write to this location, and that the path is not malformed.");
			}
		}
	});
	fileSave->setShortcut(QKeySequence::StandardKey::Save);

	QAction* fileClose = new QAction("&Close File");
	this->connect(fileClose, &QAction::triggered, this, [this]() {
		this->closeProject(this->m_Tabs->currentIndex());
	});
	fileClose->setShortcut(QKeySequence::StandardKey::Close);

	QAction* fileQuit = new QAction("&Quit");
	this->connect(fileQuit, &QAction::triggered, this, [this]() {
		QApplication::quit();
	});
	fileQuit->setShortcut(QKeySequence::StandardKey::Quit);

	// Add actions
	file->addActions({
		fileNew,
		fileOpen
	});
	file->addSeparator();
	file->addActions({
		fileSave,
		fileSaveAs
	});
	file->addSeparator();
	file->addActions({
		fileClose
	});
	file->addSeparator();
	file->addAction(fileQuit);

	QMenu* view = new QMenu("&View");
	menuBar->addMenu(view);

	view->addActions(m_ToggleViewActions);

	this->setMenuBar(menuBar);

	m_ProjectDependentActions.append({
		fileSave,
		fileSaveAs,
		fileClose
	});
}

void AppView::createStatusBar() {
	this->statusBar();
	this->statusBar()->addPermanentWidget(new StatusZoomView(m_Model));
}

i32 AppView::tabIdxFor(const ProjectModel* project) {
	for(i32 i = 0; i < m_Tabs->count(); i++) {
		ProjectView* view = (ProjectView*)m_Tabs->widget(i);
		ProjectModel* model = view->model();
		if(model == project) {
			return i;
		}
	}

	return -1;
}

void AppView::addProject(ProjectModel* project) {
	ProjectView* view = new ProjectView(project, m_Model);

	// Update the tab text in response to either pathUpdated or savedStateUpdated
	this->connect(project, &ProjectModel::pathUpdated, this, [this, project](const QString& path) {
		i32 tabIdx = this->tabIdxFor(project);
		if(tabIdx != -1) {
			this->m_Tabs->setTabText(tabIdx, QString("%1%2").arg(ProjectModel::displayNameOf(path, project->hasPath()), project->saved() ? "" : "*"));
		}
	});
	this->connect(project, &ProjectModel::savedStateUpdated, this, [this, project](bool saved) {
		i32 tabIdx = this->tabIdxFor(project);
		if(tabIdx != -1) {
			this->m_Tabs->setTabText(tabIdx, QString("%1%2").arg(project->displayName(), saved ? "" : "*"));
		}
	});

	i32 idx = m_Tabs->addTab(view, project->displayName());
	m_Tabs->setCurrentIndex(idx);

	if(m_Tabs->count() == 1) {
		for(QAction* action : m_ProjectDependentActions) {
			action->setEnabled(true);
		}
	}
}

void AppView::closeProject(int index) {
	if(index == -1) {
		return;
	}

	ProjectView* projectView = (ProjectView*)m_Tabs->widget(index);
	ProjectModel* project = projectView->model();

	if(!project->saved()) {
		QMessageBox::StandardButton response = QMessageBox::question(this,
			"Discard changes?",
			QString("This open project (%1) has unsaved changes. Discard changes and close it?").arg(project->displayName()),
			QMessageBox::StandardButton::Cancel | QMessageBox::StandardButton::Discard
		);
		if(response != QMessageBox::StandardButton::Discard) {
			return;
		}
	}

	m_Model->closeProject(project);

	m_Tabs->removeTab(index);
	projectView->deleteLater();

	if(m_Tabs->count() == 0) {
		for(QAction* action : m_ProjectDependentActions) {
			action->setEnabled(false);
		}
	}
}
