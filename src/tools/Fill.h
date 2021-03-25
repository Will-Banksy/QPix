#ifndef FILL_H
#define FILL_H

#include "Tool.h"
#include "utils/Painter.h"

/**
 * @todo write docs
 */
class Fill : public Tool {
private:
	bool fill8Way;
	int tolerance;

public:
	Fill(int id);
	~Fill() = default;
	void onMouseClicked(QMouseEvent* evt, QPoint& cPos) override;
	QList<ToolOptionWidget*> createOptions() override;
};

#endif // FILL_H
