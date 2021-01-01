#include "Application.h"
#include "UI.h"
#include <QApplication>
#include "EditorTools.h"

QList<Window*> Application::windows;

int Application::init(int argc, char *argv[]) {
	QApplication app(argc, argv);

	EditorTools::initTools();

	createWindow();

	return app.exec();
}

void Application::quit() {
	// Do some stuff beforehand perhaps, like asking all projects if they are okay to be closed
	QApplication::quit();
}

Window* Application::createWindow() {
	Window* window = new Window();
	window->show();
	windows.append(window);
	return window;
}

void Application::closeWindow(Window* window) {
	windows.removeAll(window);
}

Project* Application::currentProject() {
	return nullptr; // TODO
}

Window* Application::focusedWindow() {
	return (Window*)QApplication::activeWindow();
}
