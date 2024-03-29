#ifndef LINE_H
#define LINE_H

#include "Tool.h"
#include "utils/Painter.h"
#include "ui/Canvas.h"

/**
 * @todo write docs
 */
class Line : public Tool {
private:
	bool uniform;

public:
	Line(int id);
	~Line() override = default;
	void onMousePressed(QMouseEvent* evt, QPoint& cPos) override;
	void onMouseReleased(QMouseEvent* evt, QPoint& cPos) override;
	void onMouseDragged(QMouseEvent* evt, QPoint& cPos) override;
	QList<ToolOptionWidget*> createOptions() override;
};

#endif // LINE_H
