#include "Project.h"
#include "ui/Window.h"
#include "ProjectManager.h"
#include "ui/TabbedProjectView.h"
#include "ui/CanvasView.h"

Project::Project(Window* window_) : window(window_) {
	scene = new ProjectScene();
	scene->project = this;
	scene->canvas->project = this;
	name = "Project-";
	filepath = "Unsaved";
	viewTransform = QTransform();
	saved = true;
	viewTransform.reset(); // Reset to identity matrix
	viewScaleAmt = 1.0;
	scrollPos = QPoint(-1, -1);

	ProjectManager::registerNewProject(this);
	name += QString::number(id);

	window->ui->tabbedView->registerNewProject(this);
}

Project::~Project() {
	delete scene;
}

void Project::save() {
	saved = true;
	// TODO Actually save the file. Oh damn I'm going to have to make my own format, especially when it comes to frames and the such. Qt will be able to help hopefully
}

void Project::close(int tabIndex) {
	ProjectManager::registerCloseProject(this);
	window->ui->tabbedView->registerCloseProject(this, tabIndex);
	delete this;
}

void Project::setUnsaved() {
	this->saved = false;
	// TODO Change the tab or project name to display some indication that the project is not saved
}
