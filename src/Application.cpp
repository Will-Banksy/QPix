#include "Application.h"
#include "UI.h"
#include <QApplication>
#include "EditorTools.h"

QList<Window*> Application::windows;

int Application::init(int argc, char *argv[]) {
	QApplication app(argc, argv);

	EditorTools::initTools();

	new Window();

	return app.exec();
}

void Application::quit() {
	// Do some stuff beforehand perhaps, like asking all projects if they are okay to be closed
	for(int i = 0; i < windows.count();) {
		Window* window = windows.at(i);
		if(window->ui->windowCanClose()) {
			window->forceClose();
		} else {
			break;
		}
	}
	if(windows.isEmpty()) {
		QApplication::quit();
	}
}

void Application::registerNewWindow(Window* window) {
	window->show();
	windows.append(window);
}

void Application::registerCloseWindow(Window* window) {
	windows.removeAll(window);
}

Project* Application::currentProject() {
	return nullptr;
}

Window* Application::focusedWindow() {
	return (Window*)QApplication::activeWindow();
}
