#include "PixelEditor.h"
#include "ui/panes/ContentPane.h"
#include "ui/panes/ToolPane.h"
#include <QStyle>

PixelEditor::PixelEditor(QWidget *parent) : QMainWindow(parent) {
	setWindowTitle("QPix");

	setCentralWidget(new ContentPane());


	setDockOptions(QMainWindow::AnimatedDocks|QMainWindow::AllowTabbedDocks|QMainWindow::VerticalTabs);

	toolPane = new ToolPane();
	addDockWidget(Qt::LeftDockWidgetArea, toolPane, Qt::Vertical);

	setupMenus();
}

PixelEditor::~PixelEditor() {
	delete toolPane;
};

void PixelEditor::setupMenus() {
}