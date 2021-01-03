#ifndef APPLICATION_H
#define APPLICATION_H

#include <QList>
#include "ui/Window.h"
#include "ProjectManager.h"

class Application {
private:
	Application() = delete;

public:
	static QList<Window*> windows;
	static ProjectManager projectManager;

	static int init(int argc, char *argv[]);
	static void quit();
	/// Don't call this apart from in the Window constructor
	static void registerNewWindow(Window* window);
	/// Don't call this apart from in the Window constructor
	static void registerCloseWindow(Window* window);
	static Project* currentProject();
	static Window* focusedWindow();
};

#endif // APPLICATION_H
