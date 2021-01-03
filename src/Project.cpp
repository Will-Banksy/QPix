#include "Project.h"
#include "ui/Window.h"
#include "ProjectManager.h"
#include "ui/TabbedProjectView.h"

Project::Project(Window* window_) : window(window_) {
	scene = new ProjectScene();
	scene->project = this;
	name = "New Project";
	filepath = "Unsaved";
	viewTransform = QTransform();
	saved = false;
	viewTransform.reset(); // Reset to identity matrix
	viewScaleAmt = 1.0;

	ProjectManager::registerNewProject(this);
	window->ui->tabbedView->registerNewProject(this);
}

Project::~Project() {
	delete scene;
}

void Project::save() {
	saved = true;
	// TODO Actually save the file. Oh damn I'm going to have to make my own format, especially when it comes to frames and the such
}

void Project::close(int tabIndex) {
	ProjectManager::registerCloseProject(this);
	window->ui->tabbedView->registerCloseProject(this, tabIndex);
	delete this;
}