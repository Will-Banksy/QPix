#include "EditorTools.h"
#include "tools/Pencil.h"
#include "tools/Eraser.h"

uint EditorTools::primaryColour = 0xff000000;
uint EditorTools::secondaryColour = 0x00000000;
QList<Tool*> EditorTools::tools = QList<Tool*>();
Tool* EditorTools::selectedTool = nullptr;

void EditorTools::initTools(Canvas* canvas) {
	tools.append(new Pencil(0, canvas));
	tools.append(new Eraser(1, canvas));
	selectedTool = tools[0];
}


EditorTools::EditorTools() {
}

EditorTools::~EditorTools() {
}