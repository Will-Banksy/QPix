#ifndef PROJECT_H
#define PROJECT_H

#include "ui/Canvas.h"
#include "ui/ProjectScene.h"

class Window;

class Project {
public:
	Project(Window* window_);
	~Project();

	ProjectScene* scene; // Owns this object
	Window* window; // Does not own this
	QString name;
	QString filepath;
	QTransform viewTransform;
	bool saved;
	int id;

	void save();
	void close(int tabIndex = -1);
};

#endif // PROJECT_H
