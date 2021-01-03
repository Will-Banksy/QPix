#include "ProjectScene.h"
#include "ui/Canvas.h"
#include <QGraphicsSceneWheelEvent>
#include <QPainter>
#include <QGraphicsView>

ProjectScene::ProjectScene() : QGraphicsScene() {
	canvas = new Canvas();
	addItem(canvas);
}

ProjectScene::~ProjectScene() {
	for(QGraphicsView* view : views()) {
		view->setScene(nullptr);
	}
	delete canvas;
}