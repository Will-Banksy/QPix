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
#include <QTableWidget>
#include <QDialog>
#include <QTreeView>
#include <QStandardItemModel>
#include <QPushButton>

UI::UI(Window* window) : window(window) {
}

UI::~UI() {
	// TODO Need to do some deleting
}

void UI::setupUI() {
	if(!window) {
		return;
	}
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
		newTab();

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
	file->addSeparator();
	file->addAction(actions["Close Window"]);
	file->addAction(actions["Close Tab"]);
	file->addSeparator();
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

	QAction* closeWindowAct = new QAction(tr("Clos&e Window"));
	closeWindowAct->setShortcut(QKeySequence(""));
	// Status tip
	connect(closeWindowAct, &QAction::triggered, this, &UI::closeWindow);
	actions.insert("Close Window", closeWindowAct);

	QAction* closeTabAct = new QAction(tr("&Close Tab"));
	closeTabAct->setShortcut(QKeySequence::Close);
	// Status tip
	connect(closeTabAct, &QAction::triggered, this, &UI::closeTab);
	actions.insert("Close Tab", closeTabAct);
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
	new Window();
}

void UI::newTab() {
	new Project(window);
}

void UI::closeWindow() {
	window->close(); // This will check if windowCanClose
}

void UI::closeTab() {
	tabbedView->handleTabClose(tabbedView->tabbar->currentIndex());
}

void UI::switchToolUI(int selectedTool) {
	toolButtons.at(selectedTool)->setChecked(true);
	toolConfigStack->setCurrentIndex(selectedTool);
}

bool UI::windowCanClose() {
	QList<Project*> projects = tabbedView->getProjects();
	QList<Project*> unsavedProjects;
	for(int i = 0; i < projects.count(); i++) {
		if(!projects.at(i)->saved) {
			unsavedProjects.append(projects.at(i));
		}
	}
	if(unsavedProjects.count() == 0) {
		return true;
	} else {
		QDialog diag(window);
		diag.setWindowTitle("Save Projects - QPix");

		bool returnValue = false;
		QList<QStandardItem*> entries;
		QTreeView* treeView = new QTreeView();

		QStandardItemModel* model = new QStandardItemModel();
		model->setHorizontalHeaderLabels({ "Project Name", "Filepath" });

		for(int i = 0; i < unsavedProjects.count(); i++) {
			QStandardItem* checkItem = new QStandardItem(unsavedProjects.at(i)->name);
			checkItem->setCheckable(true);
			checkItem->setCheckState(Qt::Checked);

			entries.append(checkItem);

			std::cout << "[DIAG] ProjectId: " << unsavedProjects.at(i)->id << std::endl;

			model->appendRow({ checkItem, new QStandardItem(unsavedProjects.at(i)->filepath) });
		}

		treeView->setModel(model);
		treeView->resizeColumnToContents(0);
		treeView->resizeColumnToContents(1);

		QPushButton* saveSelected = new QPushButton(QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton), "Save");
		saveSelected->setAutoDefault(true);
		connect(saveSelected, &QPushButton::clicked, [&entries, &unsavedProjects, &returnValue, &diag]() {
			for(int i = 0; i < entries.count(); i++) {
				if(entries.at(i)->checkState() == Qt::Checked) {
					unsavedProjects.at(i)->save();
				}
			}
			returnValue = true;
			diag.close();
		});

		QPushButton* discardAll = new QPushButton(QApplication::style()->standardIcon(QStyle::SP_DialogDiscardButton), "Discard All");
		discardAll->setAutoDefault(false);
		connect(discardAll, &QPushButton::clicked, [&returnValue, &diag]() {
			returnValue = true;
			diag.close();
		});

		QPushButton* cancel = new QPushButton(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton), "Cancel");
		cancel->setAutoDefault(false);
		connect(cancel, &QPushButton::clicked, [&returnValue, &diag]() {
			returnValue = false;
			diag.close();
		});

		QVBoxLayout* layout = new QVBoxLayout();
		layout->addWidget(treeView);

		QHBoxLayout* layout2 = new QHBoxLayout();
		layout2->setAlignment(Qt::AlignRight);
		layout2->addWidget(saveSelected);
		layout2->addWidget(discardAll);
		layout2->addWidget(cancel);

		layout->addLayout(layout2);

		diag.setLayout(layout);

		diag.exec(); // Show the dialog as a modal dialog - blocks user input to all other parts of the Qt application
		return returnValue;
	}
}
