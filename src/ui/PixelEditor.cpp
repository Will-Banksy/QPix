#include "PixelEditor.h"
#include "ui/panes/CanvasPane.h"
#include "ui/panes/ToolPane.h"
#include <QStyle>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPaintEvent>

PixelEditor::PixelEditor(QWidget *parent) : QMainWindow(parent) {
	setWindowTitle("QPix");

	setCentralWidget(new CanvasPane());

	setDockOptions(QMainWindow::AnimatedDocks|QMainWindow::AllowTabbedDocks|QMainWindow::VerticalTabs);

	toolPane = new ToolPane();
	addDockWidget(Qt::LeftDockWidgetArea, toolPane, Qt::Vertical);

	setupMenus();
}

PixelEditor::~PixelEditor() {
	delete toolPane;
};

void PixelEditor::setupMenus() {
	// Add the menu here
}