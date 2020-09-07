#ifndef LINE_H
#define LINE_H

#include "Tool.h"
#include "utils/Algorithms.h"
#include "ui/Canvas.h"

/**
 * @todo write docs
 */
class Line : public Tool {
	public:
		Line(int id, Canvas* canvas);
		~Line() override = default;
		void onMousePressed(QMouseEvent* evt, QPoint& cPos) override;
		void onMouseReleased(QMouseEvent* evt, QPoint& cPos) override;
		void onMouseDragged(QMouseEvent* evt, QPoint& cPos) override;

	private:
		AlgoAction toolAction;
};

#endif // LINE_H
