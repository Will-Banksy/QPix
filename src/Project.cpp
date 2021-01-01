#include "Project.h"
#include "Window.h"

Project::Project(Window* window_, int id) : window(window_), id(id) {
	scene = new ProjectScene();
	scene->project = this;
	name = "New Project";
	viewTransform = QTransform();
	saved = false;
	tabId = 0;
}

Project::~Project() {
	delete scene;
}