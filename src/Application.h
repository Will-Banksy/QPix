#ifndef APPLICATION_H
#define APPLICATION_H

#include <QList>
#include "Window.h"
#include "ProjectManager.h"

class Application {
private:
	Application() = delete;

public:
	static QList<Window*> windows;
	static ProjectManager projectManager;

	static int init(int argc, char *argv[]);
	static void quit();
	static Window* createWindow();
	static void closeWindow(Window* window);
	static Project* currentProject();
	static Window* focusedWindow();
};

#endif // APPLICATION_H
