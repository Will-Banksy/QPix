#include "EditorTools.h"
#include "tools/Pencil.h"
#include "tools/Eraser.h"
#include "tools/Fill.h"
#include "tools/Line.h"

uint EditorTools::primaryColour = 0xff000000;
uint EditorTools::secondaryColour = 0x00000000;
QList<Tool*> EditorTools::tools = QList<Tool*>();
Tool* EditorTools::selectedTool = nullptr;
Brush* EditorTools::brush = new VariableBrush();
ushort EditorTools::brushWidth = 1;
ushort EditorTools::brushHeight = 1;

void EditorTools::initTools(Canvas* canvas) {
	tools.append(new Pencil(0, canvas));
	tools.append(new Eraser(1, canvas));
	tools.append(new Fill(2, canvas));
	tools.append(new Line(3, canvas));
	selectedTool = tools.at(0);
}

void EditorTools::setBrushSize(ushort size) {
	brushWidth = size;
	brushHeight = size;
}

void EditorTools::setBrushSize(ushort width, ushort height) {
	brushWidth = width;
	brushHeight = height;
}


/*EditorTools::EditorTools() {
}

EditorTools::~EditorTools() {
	// Don't need to delete selectedTool - when the QList deletes itself it should take care of deleting all the tools. Right?
}*/