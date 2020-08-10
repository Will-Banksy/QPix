#ifndef CANVASPANE_H
#define CANVASPANE_H

#include <QGraphicsView>
#include "ui/Canvas.h"

/**
 * @todo write docs
 */
class CanvasPane : public QGraphicsView {
	public:
		explicit CanvasPane(QWidget* parent = nullptr);
		~CanvasPane();
		Canvas* canvas;

	protected:
		void wheelEvent(QWheelEvent* event) override;
		void mousePressEvent(QMouseEvent* event) override;
		void mouseReleaseEvent(QMouseEvent* event) override;
		void mouseMoveEvent(QMouseEvent* event) override;

	private:
		bool mouseDown;
		bool hasMovedMouse;
		bool ignoreRelease;

		double scaleAmt; // Keeps track of the current scale
};

#endif // CANVASPANE_H
