#ifndef SCROLLHAND_H
#define SCROLLHAND_H

#include "Tool.h"

class Window;

/**
 * @todo write docs
 */
class ScrollHand : public Tool {
private:
	Window* window;

public:
	ScrollHand(int id);
	~ScrollHand() override;
	virtual void onMousePressed(QMouseEvent* evt, QPoint& cPos) override;
	virtual void onMouseReleased(QMouseEvent* evt, QPoint& cPos) override;
};

#endif // SCROLLHAND_H
