#include "Window.h"
#include "Application.h"
#include <QCloseEvent>
#include "TabbedProjectView.h"

Window::Window() {
	ui = new UI(this);
	ui->setupUI();
	dontCheckClose = false;
	Application::registerNewWindow(this);
}

Window::~Window() {
	delete ui;
}

void Window::closeEvent(QCloseEvent* event) {
	if(dontCheckClose || ui->windowCanClose()) { // If dontCheckClose is true, then ui->windowCanClose is not checked
		// Can save layout state here
		Application::registerCloseWindow(this);
		closeProjects();
		event->accept();
		delete this;
	} else {
		event->ignore();
	}
}

void Window::forceClose() {
	dontCheckClose = true;
	close();
}

void Window::closeProjects() {
	ui->tabbedView->closeAllProjects();
}
