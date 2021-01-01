#include "UI.h"
#include <QAction>
#include <QApplication>
#include "Window.h"
#include <QMenuBar>
#include <QMenu>
#include "Application.h"
#include "ui/CanvasView.h"
#include "ui/panes/ToolPane.h"
#include "EditorTools.h"
#include <QToolBar>
#include <QStackedWidget>
#include "ui/TabbedProjectView.h"

UI::UI(Window* window) : window(window) {
	if(window) {
		setupUI();
	}
}

UI::~UI() {
}

void UI::setupUI() {
	window->setWindowTitle("QPix");
	window->setDockOptions(QMainWindow::AnimatedDocks|QMainWindow::AllowTabbedDocks|QMainWindow::VerticalTabs);
// 	window->statusBar();
	createActions();
	createMenus();
	createDocks();
	createToolbars();
	// Create other stuff too

	{
		canvasView = new CanvasView();

		tabbedView = new TabbedProjectView(canvasView, { });
		ProjectManager::createProject(window);

		window->setCentralWidget(tabbedView);

		canvasView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
		canvasView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
// 		canvasView->setStatusTip("Canvas");
	}
}

void UI::createMenus() {
	QMenuBar* menuBar = new QMenuBar();
	window->setMenuBar(menuBar);
	menuBar->setNativeMenuBar(true); // If possible will use this as the native menubar

	QMenu* file = menuBar->addMenu(QObject::tr("&File"));
	file->addSeparator();
	file->addAction(actions["New Window"]);
	file->addAction(actions["New Tab"]);
	file->addAction(actions["Quit"]);

	QMenu* view = menuBar->addMenu(QObject::tr("&View"));
	QMenu* showToolViews = view->addMenu(QObject::tr("Show Tool Views"));
	showToolViews->addAction(actions["Show Tools"]);
}

void UI::createActions() {
	QAction* quitAct = new QAction(QObject::tr("&Quit"), window);
	quitAct->setShortcut(QKeySequence::Quit);
// 		quitAct->setStatusTip(tr("Quit the application"));
	connect(quitAct, &QAction::triggered, this, &Application::quit, Qt::QueuedConnection); // Using a QueuedConnection because QCoreApplication::quit() recommends it
	actions.insert("Quit", quitAct);

	QAction* showToolsAct = new QAction(tr("Show Tools"), window);
// 		showToolsAct->setStatusTip(tr("Show the Tools tool view"));
	connect(showToolsAct, &QAction::triggered, this, &UI::showToolsDock);
	actions.insert("Show Tools", showToolsAct);

	QAction* newWindowAct = new QAction(tr("&New Window"));
	newWindowAct->setShortcut(QKeySequence::New);
	// Status tip
	connect(newWindowAct, &QAction::triggered, this, &UI::newWindow);
	actions.insert("New Window", newWindowAct);

	QAction* newTabAct = new QAction(tr("New &Tab"));
	newTabAct->setShortcut(QKeySequence::AddTab);
	// Status tip
	connect(newTabAct, &QAction::triggered, this, &UI::newTab);
	actions.insert("New Tab", newTabAct);
}

void UI::createDocks() {
	{
		toolDock = new ToolPane();
		QWidget* widget = new QWidget();
		FlowLayout2* layout = new FlowLayout2();
		for(Tool* tool : EditorTools::tools) {
			ToolButton* btn = new ToolButton(tool);
			if(tool == EditorTools::selectedTool) {
				btn->setChecked(true);
			}
			layout->addWidget(btn);
			toolButtons.append(btn);
		}
		QVBoxLayout* outerLayout = new QVBoxLayout();
		outerLayout->setAlignment(Qt::AlignTop);
		outerLayout->addLayout(layout);
		widget->setLayout(outerLayout);
		toolDock->setWidget(widget);
		window->addDockWidget(Qt::LeftDockWidgetArea, toolDock, Qt::Vertical);
	}
}

void UI::createToolbars() { // TODO Make it so that when the option widgets state changes (like a ToolOptionBool being ticked) then the option widget is updated in all windows
	QToolBar* toolbar = new QToolBar("Tool Config Toolbar");
	toolConfigStack = new QStackedWidget();

	for(Tool* tool : EditorTools::tools) {
		QWidget* container = new QWidget();
		QHBoxLayout* layout = new QHBoxLayout();
		layout->setAlignment(Qt::AlignLeft); // Don't stretch the widgets all the way across. Align them to the left
		for(ToolOptionWidget* opWidget : tool->createOptions()) {
			switch(opWidget->optionType()) {
				case TOT_BOOL:
					layout->addWidget((ToolOptionBool*)opWidget);
					break;

				case TOT_MULTI:
					layout->addWidget((ToolOptionMulti*)opWidget);
					break;

				default:
					// Kill myself
					break;
			}
		}
		container->setLayout(layout);
		toolConfigStack->insertWidget(tool->id, container);
	}

	toolbar->addWidget(toolConfigStack);
	window->addToolBar(Qt::TopToolBarArea, toolbar);
	toolConfigStack->setCurrentIndex(EditorTools::selectedTool->id);
}

void UI::showToolsDock() {
	toolDock->show();
}

void UI::newWindow() {
	Application::createWindow();
}

void UI::newTab() {
	Project* project = ProjectManager::createProject(window);
// 	tabbedView->addProject(project);
}

void UI::switchToolUI(int selectedTool) {
	toolButtons.at(selectedTool)->setChecked(true);
	toolConfigStack->setCurrentIndex(selectedTool);
}