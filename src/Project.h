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
	bool saved;
	int id;

	QTransform viewTransform;
	double viewScaleAmt;
	QPoint scrollPos;

	void save();
	void close(int tabIndex = -1);
	/// Should always call this function instead of changing the variable directly
	void setUnsaved();
};

#endif // PROJECT_H
