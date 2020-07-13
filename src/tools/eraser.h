#ifndef ERASER_H
#define ERASER_H

#include "tool.h"
#include "src/utils/algorithms.h"

/**
 * @todo write docs
 */
class Eraser : public Tool {
	private:
		AlgoAction toolAction;
		uint transparent = 0x00000000;

	public:
		Eraser(int id, Canvas* canvas);
		~Eraser() override; // Need to override destructor: https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors#461224
		void onMousePressed(QMouseEvent* evt, QPoint& cPos) override;
		void onMouseReleased(QMouseEvent* evt, QPoint& cPos) override;
// 		void onMouseClicked(QMouseEvent* evt, QPoint& cPos) override;
		void onMouseDragged(QMouseEvent* evt, QPoint& cPos) override;
};

#endif // ERASER_H
