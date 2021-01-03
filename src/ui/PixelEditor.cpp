/*#include "PixelEditor.h"
#include "ui/panes/ToolPane.h"
#include <QStyle>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QMenuBar>
#include <QApplication>
#include <QStatusBar>
#include <QToolBar>
#include <QStackedWidget>
#include "tools/Tool.h"
#include "EditorTools.h"
#include <typeindex>
#include <typeinfo>

PixelEditor::PixelEditor(QWidget *parent) : QMainWindow(parent) {
	setWindowTitle("QPix");

// 	setCentralWidget(new CanvasPane()); // This calls EditorTools::initTools

	addToolBars();

	setDockOptions(QMainWindow::AnimatedDocks|QMainWindow::AllowTabbedDocks|QMainWindow::VerticalTabs);

	toolPane = new ToolPane();
	addDockWidget(Qt::LeftDockWidgetArea, toolPane, Qt::Vertical);

// 	statusBar()->setSizeGripEnabled(false); // Will probably add this back at some point
	setupMenus();
}

PixelEditor::~PixelEditor() {
	delete toolPane;
};

void PixelEditor::setupMenus() {
	createActions();

	QMenuBar* menuBar = new QMenuBar();
	setMenuBar(menuBar);
	menuBar->setNativeMenuBar(true); // If possible will use this as the native menubar

	QMenu* file = menuBar->addMenu(tr("&File"));
	file->addSeparator();
	file->addAction(quitAct);

	QMenu* view = menuBar->addMenu(tr("&View"));
	QMenu* showToolViews = view->addMenu(tr("Show Tool Views"));
	showToolViews->addAction(showToolsAct);
}

void PixelEditor::createActions() { // Status bar tips don't seem to be working
	quitAct = new QAction(tr("&Quit"), this);
	quitAct->setShortcut(QKeySequence::Quit);
// 	quitAct->setStatusTip(tr("Quit the application"));
	connect(quitAct, &QAction::triggered, this, &PixelEditor::quit, Qt::QueuedConnection); // Using a QueuedConnection because QCoreApplication::quit() recommends it

	showToolsAct = new QAction(tr("Show Tools"), this);
// 	showToolsAct->setStatusTip(tr("Show the Tools tool view"));
	connect(showToolsAct, &QAction::triggered, this, &PixelEditor::showTools);
}

void PixelEditor::quit() {
	// Here I can do stuff before quitting
	QApplication::quit();
}

void PixelEditor::showTools() {
	toolPane->setVisible(true);
}

void PixelEditor::addToolBars() {
	QToolBar* toolbar = new QToolBar("Toolbar");
	QStackedWidget* stack = new QStackedWidget();

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
					std::cout << "Argh. Why???" << std::endl;
					break;
			}
// 			std::type_index type = typeid(opWidget);
// 			std::cout << "About to add a bool or multi widget?" << std::endl;
// 			if(type == typeid(ToolOptionBool)) {
// 				layout->addWidget(&dynamic_cast<ToolOptionBool&>(opWidget));
// 				std::cout << "Added bool widget" << std::endl;
// 			} else if(type == typeid(ToolOptionMulti)) {
// 				layout->addWidget(&dynamic_cast<ToolOptionMulti&>(opWidget));
// 				std::cout << "Added multi widget" << std::endl;
// 			} else {
// 				std::cout << "Nope!" << std::endl;
// 			}
		}
		container->setLayout(layout);
		stack->addWidget(container);
	}

	toolbar->addWidget(stack);
	addToolBar(Qt::TopToolBarArea, toolbar);

// 	QToolBar* toolbar = new QToolBar();
// 	toolbar->setMovable(false);
// 	QStackedWidget* stack = new QStackedWidget();
//
// 	QComboBox* box = new QComboBox();
// 	box->addItem("Item 1");
// 	box->addItem("Item 2");
// 	box->addItem("Item 3");
// 	stack->addWidget(box);
//
// 	toolbar->addWidget(stack);
// 	addToolBar(Qt::TopToolBarArea, toolbar);
}*/