#include "PixelEditor.h"
#include "ui/panes/ContentPane.h"
#include "ui/panes/ToolPane.h"

PixelEditor::PixelEditor(QWidget *parent) : QMainWindow(parent) {
	setCentralWidget(new ContentPane());

	setWindowTitle("QPix");

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