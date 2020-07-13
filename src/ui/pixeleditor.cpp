#include "pixeleditor.h"
#include "src/ui/panes/contentpane.h"

PixelEditor::PixelEditor(QWidget *parent) : QMainWindow(parent) {
	setCentralWidget(new ContentPane());
}

PixelEditor::~PixelEditor() = default;
