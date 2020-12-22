#include "ProjectScene.h"
#include "ui/Canvas.h"
#include <QGraphicsSceneWheelEvent>
#include <QPainter>

ProjectScene::ProjectScene() : QGraphicsScene() {
	canvas = new Canvas();
	addItem(canvas);
}