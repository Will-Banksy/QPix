#ifndef PENCIL_H
#define PENCIL_H

#include "Tool.h"
#include "utils/Painter.h"

/**
 * @todo write docs
 */
class Pencil : public Tool {
	private:
		QList<QPair<QPoint, uint>> currentStroke;

	public:
		Pencil(int id);
		~Pencil() override; // Need to override destructor: https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors#461224
		void onMousePressed(QMouseEvent* evt, QPoint& cPos) override;
		void onMouseReleased(QMouseEvent* evt, QPoint& cPos) override;
// 		void onMouseClicked(QMouseEvent* evt, QPoint& cPos) override;
		void onMouseDragged(QMouseEvent* evt, QPoint& cPos) override;
};

#endif // PENCIL_H
