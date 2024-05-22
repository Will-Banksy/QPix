#include "ScrollHand.h"
#include "Application.h"
#include "ui/CanvasView.h"

ScrollHand::ScrollHand(int id) : Tool(id) {
	name = "Scroll Hand";
	description = "Moves the view";
	keyShortcut = " ";
	iconPath = ":/tools/move.png";
}

ScrollHand::~ScrollHand() {
}

void ScrollHand::onMousePressed(QMouseEvent* evt, QPoint& cPos) {
	window = Application::focusedWindow();
	if(window) {
		window->ui->canvasView->setDragMode(QGraphicsView::ScrollHandDrag);
	}
}

void ScrollHand::onMouseReleased(QMouseEvent* evt, QPoint& cPos) {
	if(window) {
		window->ui->canvasView->setDragMode(QGraphicsView::NoDrag);
	}
}
