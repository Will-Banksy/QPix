#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "tools/Tool.h"
#include "utils/Brush.h"

namespace utils {
	typedef unsigned int uint;
// 	typedef std::function<void(int, int, uint, Tool::ToolSettings&)> AlgoAction;

	/**
	* This class contains various functions to draw to any QImage.
	* A bit like a static QPainter but much more specific and static
	*/
	class Painter { // TODO: Need to change how Algorithms handles drawing I think
		public:
			static void drawLine(QImage& target, int startX, int startY, int endX, int endY, uint col, const Brush& brush);
			static void drawUniformLine(QImage& target, int startX, int startY, int endX, int endY, uint col, const Brush& brush);
			static void drawLinePixelPerfect(QImage& target, int startX, int startY, int endX, int endY, uint col, const Brush& brush, QList<QPair<QPoint, uint>>& currentStroke);
			static void fillArea(QImage& target, int startX, int startY, uint col, const Brush& brush, int tolerance = 0, bool fill8Way = false);

		private:
			static bool checkFillValidity(QImage& target, int i, int j, uint targetColour, int tolerance, bool** filled);
			static bool canFill(uint targetColour, uint colour, int tolerance);
			static void applyBrush(int i, int j, uint col, Tool::ToolSettings& settings);
			static uint getPixel(QImage& target, int i, int j);
	};
}

#endif // ALGORITHMS_H
