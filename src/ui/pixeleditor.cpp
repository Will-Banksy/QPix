#include "pixeleditor.h"
#include "contentpane.h"

PixelEditor::PixelEditor(QWidget *parent) : QMainWindow(parent) {
	setCentralWidget(new ContentPane());
}

PixelEditor::~PixelEditor() = default;
