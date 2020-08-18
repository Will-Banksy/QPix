#include "PixelEditor.h"
#include "ui/panes/CanvasPane.h"
#include "ui/panes/ToolPane.h"
#include <QStyle>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QMenuBar>
#include <QApplication>
#include <QStatusBar>

PixelEditor::PixelEditor(QWidget *parent) : QMainWindow(parent) {
	setWindowTitle("QPix");

	setCentralWidget(new CanvasPane());

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

	QMenuBar* menuBar = new QMenuBar(this);

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