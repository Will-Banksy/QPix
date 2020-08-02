#include "pixeleditor.h"
#include "src/ui/panes/contentpane.h"
#include "src/ui/panes/toolpane.h"

PixelEditor::PixelEditor(QWidget *parent) : QMainWindow(parent) {
	setCentralWidget(new ContentPane());

	setWindowTitle("QPix");

	setDockOptions(QMainWindow::AnimatedDocks|QMainWindow::AllowTabbedDocks|QMainWindow::VerticalTabs);

	addDockWidget(Qt::LeftDockWidgetArea, new ToolPane(), Qt::Vertical);
	addDockWidget(Qt::RightDockWidgetArea, new QDockWidget("Another Dock Widget"), Qt::Vertical);
}

PixelEditor::~PixelEditor() = default;
