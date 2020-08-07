#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "tools/Tool.h"

typedef unsigned int uint;
typedef std::function<void(int, int, uint, Tool::ToolSettings&)> AlgoAction;

/**
 * @todo write docs
 */
class Algorithms {
	public:
		static void plotLine(int startX, int startY, int endX, int endY, uint col, Tool::ToolSettings& settings, AlgoAction& action);
};

#endif // ALGORITHMS_H
