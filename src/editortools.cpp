#include "editortools.h"
#include "src/tools/pencil.h"

uint EditorTools::primaryColour = 0xff000000;
uint EditorTools::secondaryColour = 0x00000000;
QList<Tool*> EditorTools::tools = QList<Tool*>();
Tool* EditorTools::selectedTool = nullptr;

void EditorTools::initTools(Canvas* canvas) {
	tools.append(new Pencil(0, canvas));
	selectedTool = tools[0];
}


EditorTools::EditorTools() {
}

EditorTools::~EditorTools() {
}
