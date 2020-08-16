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
		static void fillArea(Canvas& canvas, int startX, int startY, uint col, Tool::ToolSettings& settings, AlgoAction& action);

	private:
		static bool checkFillValidity(Canvas& canvas, int i, int j, uint targetColour, int tolerance, bool** filled);
		static bool canFill(uint targetColour, uint colour, int tolerance);
};

#endif // ALGORITHMS_H
