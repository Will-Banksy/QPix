#ifndef CANVASPANE_H
#define CANVASPANE_H

#include <QGraphicsView>
#include "src/ui/canvas.h"

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
		void mouseMoveEvent(QMouseEvent* event) override; // Goddamnit it seems that overriding mouseMoveEvent makes zooming just zoom into the corner (regardless of if mouse tracking is enabled)

	private:
		bool mouseDown;
		bool hasMovedMouse;
		bool ignoreRelease;

		double scaleNumber; // --
};

#endif // CANVASPANE_H
