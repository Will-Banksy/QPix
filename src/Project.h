#ifndef PROJECT_H
#define PROJECT_H

#include "ui/Canvas.h"
#include "ui/ProjectScene.h"

class Window;

class Project {
public:
	Project(Window* window_, int id);
	~Project();

	ProjectScene* scene; // Owns this object
	Window* window; // Does not own this
	QString name;
	QTransform viewTransform;
	bool saved;
	int tabId;
	int id;
};

#endif // PROJECT_H
