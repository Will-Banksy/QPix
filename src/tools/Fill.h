#ifndef FILL_H
#define FILL_H

#include "Tool.h"
#include "utils/Algorithms.h"

/**
 * @todo write docs
 */
class Fill : public Tool {
	public:
		Fill(int id, Canvas* canvas);
		~Fill() = default;
		void onMouseClicked(QMouseEvent* evt, QPoint& cPos) override;

	private:
		AlgoAction toolAction;
};

#endif // FILL_H
