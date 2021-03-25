#include "EditorTools.h"
#include "tools/Pencil.h"
#include "tools/Eraser.h"
#include "tools/Fill.h"
#include "tools/Line.h"
#include "tools/ScrollHand.h"
#include "ui/Window.h"
#include "Application.h"

uint EditorTools::primaryColour = 0xff000000;
uint EditorTools::secondaryColour = 0x00000000;
QList<Tool*> EditorTools::tools = QList<Tool*>();
Tool* EditorTools::selectedTool = nullptr;
Brush* EditorTools::brush = new VariableBrush();
ushort EditorTools::brushWidth = 1;
ushort EditorTools::brushHeight = 1;
bool EditorTools::toolSwitchlock = false;

void EditorTools::initTools() {
	tools.append(new Pencil(0));
	tools.append(new Eraser(1));
	tools.append(new Fill(2));
	tools.append(new Line(3));
	tools.append(new ScrollHand(4));
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

void EditorTools::switchTool(int selectedToolIndex) {
	if(toolSwitchlock) {
		return;
	}
	selectedTool = tools.at(selectedToolIndex);
	for(Window* window : Application::windows) {
		window->ui->switchToolUI(selectedToolIndex);
	}
}