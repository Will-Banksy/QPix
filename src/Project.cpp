#include "Project.h"
#include "Window.h"

Project::Project(Window* window_) : window(window_) {
	scene = new ProjectScene();
	scene->project = this;
}

Project::~Project() {
	delete scene;
}