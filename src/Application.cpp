#include "Application.h"
#include "ui/UI.h"
#include <QApplication>
#include "EditorTools.h"
#include "Icons.h"

QList<Window*> Application::windows;

int Application::init(int argc, char *argv[]) {
	QApplication app(argc, argv);

// 	app.setStyle("fusion");

	EditorTools::initTools();

	QObject::connect(&app, &QGuiApplication::paletteChanged, [](const QPalette& palette) {
		QColor iconColour = palette.text().color();
		Icons::setIconsColour(iconColour);
	});

	Icons::loadIcons(app.palette().text().color());

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
