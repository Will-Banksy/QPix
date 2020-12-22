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
	createActions();
	createMenus();
	createDocks();
	createToolbars();
	// Create other stuff too

	canvasPane = new CanvasView();
	window->setCentralWidget(canvasPane);
	{
		Project* project = ProjectManager::createProject(window);

		canvasPane->setScene(project->scene);

		canvasPane->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
		canvasPane->setResizeAnchor(QGraphicsView::AnchorViewCenter);
	}
}

void UI::createMenus() {
	QMenuBar* menuBar = new QMenuBar();
	window->setMenuBar(menuBar);
	menuBar->setNativeMenuBar(true); // If possible will use this as the native menubar

	QMenu* file = menuBar->addMenu(QObject::tr("&File"));
	file->addSeparator();
	file->addAction(actions["Quit"]);
	file->addAction(actions["New Window"]);

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

void UI::createToolbars() {
	QToolBar* toolbar = new QToolBar("Tool Config Toolbar");
	toolConfigStack = new QStackedWidget();

	for(Tool* tool : EditorTools::tools) {
		QWidget* container = new QWidget();
		QHBoxLayout* layout = new QHBoxLayout();
		layout->setAlignment(Qt::AlignLeft); // Don't stretch the widgets all the way across. Align them to the left
		for(ToolOptionWidget* opWidget : tool->options) {
			switch(opWidget->optionType()) {
				case TOT_BOOL:
					layout->addWidget((ToolOptionBool*)opWidget);
					break;

				case TOT_MULTI:
					layout->addWidget((ToolOptionMulti*)opWidget);
					break;

				default:
					std::cout << "Argh. Why???" << std::endl;
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

void UI::switchToolUI(int selectedTool) {
	toolButtons.at(selectedTool)->setChecked(true);
	toolConfigStack->setCurrentIndex(selectedTool);
}