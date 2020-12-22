#ifndef PROJECTSCENE_H
#define PROJECTSCENE_H

#include <QGraphicsScene>

class Project;
class Canvas;

class ProjectScene : public QGraphicsScene {
	Q_OBJECT

public:
	ProjectScene();

	Project* project;
	Canvas* canvas;
};

#endif // PROJECTSCENE_H
