#ifndef WINDOW_H
#define WINDOW_H

#include "UI.h"
#include <QMainWindow>

class Project;

class Window : public QMainWindow {
	Q_OBJECT

public:
	Window();
	virtual ~Window();

	UI* ui; // Owns this object

	Project* activeProject();
	void forceClose();
	void closeProjects();

protected:
	void closeEvent(QCloseEvent* event) override;

private:
	bool dontCheckClose;
};

#endif // WINDOW_H
