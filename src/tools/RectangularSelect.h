#ifndef RECTANGULARSELECT_H
#define RECTANGULARSELECT_H

#include "Tool.h"

/**
 * @todo write docs
 */
class RectangularSelect : public Tool {
public:
	RectangularSelect(int id);

	void onMousePressed(QMouseEvent* evt, QPoint& cPos) override;
	void onMouseReleased(QMouseEvent* evt, QPoint& cPos) override;
	void onMouseClicked(QMouseEvent* evt, QPoint& cPos) override;
	void onMouseDragged(QMouseEvent* evt, QPoint& cPos) override;

	bool isSelector() override;
};

#endif // RECTANGULARSELECT_H
