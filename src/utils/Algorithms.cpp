#include "Algorithms.h"
#include <QQueue>
#include <QPoint>
#include <QPointF>
#include "Helper.h"
#include "Colour.h"
#include <cmath>

namespace utils {
	inline int abs(int val) {
		if(val < 0) {
			return val * -1;
		}
		return val;
	}

	inline float sq(float val) {
		return val * val;
	}

	inline float distance(float x0, float y0, float x1, float y1) {
		return sqrtf(sq(x0 - x1) + sq(y0 - y1));
	}

	void Algorithms::plotLine(int startX, int startY, int endX, int endY, uint col, Tool::ToolSettings& settings, AlgoAction& action, bool uniformLine) {
		int& x0 = startX;
		int& y0 = startY;
		int& x1 = endX;
		int& y1 = endY;

		if(!uniformLine) {
			int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
			int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
			int err = dx+dy, e2; /* error value e_xy */

			for(;;){  /* loop */
				// Call the function with the () operator
				action(x0, y0, col, settings);
				if (x0==x1 && y0==y1) break;
				e2 = 2*err;
				if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
					if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
			}
		} else {
			float dx = abs(x1 - x0) + 1;
			float dy = abs(y1 - y0) + 1;

			float sx = (x0 < x1) ? 1 : -1;
			float sy = (y0 < y1) ? 1 : -1;

			float ratio = fmax(dx, dy) / fmin(dx, dy);

			int pixelStep = round(ratio);

			if (pixelStep > fmin(dx, dy)) {
				pixelStep = std::numeric_limits<double>::infinity();//(int)Double.POSITIVE_INFINITY;
			}

			float maxDistance = (float)distance(x0, y0, x1, y1);

			int x = x0;
			int y = y0;
			int i = 0;
			while (true) {
				i++;

				action(x, y, col, settings);
				if (distance(x0, y0, x, y) >= maxDistance) {
					break;
				}

				bool isAtStep = i % pixelStep == 0;
				if (dx >= dy || isAtStep) {
					x += sx;
				}
				if (dy >= dx || isAtStep) {
					y += sy;
				}
			}
		}
	}

	// I would say this is fast enough, for now
	void Algorithms::fillArea(Canvas& canvas, int startX, int startY, uint col, Tool::ToolSettings& settings, AlgoAction& action) {
		bool** filled = utils::create2DArray<bool>(canvas.buffer->width(), canvas.buffer->height(), false);

		int tolerance = settings.tolerance;

		uint targetColour = canvas.buffer->pixel(startX, startY);

// 		uint* bytes = (uint*)canvas.buffer->bits(); // Get the bits for fast writing/reading

		QQueue<QPoint> q;

		// enqueue(...) to push, dequeue() to pop
		q.enqueue(QPoint(startX, startY));

		while(!q.isEmpty()) {
			QPoint p = q.dequeue();

// 			*colAt(bytes, canvas.buffer->width(), p.x(), p.y()) = col; // This should be roughly equivalent to canvas.buffer->setPixel(i, j, col)
			action(p.x(), p.y(), col, settings);

			if(checkFillValidity(canvas, p.x() + 1, p.y(), targetColour, tolerance, filled)) {
				q.enqueue(QPoint(p.x() + 1, p.y()));
				filled[p.x() + 1][p.y()] = true;
			}
			if(checkFillValidity(canvas, p.x() - 1, p.y(), targetColour, tolerance, filled)) {
				q.enqueue(QPoint(p.x() - 1, p.y()));
				filled[p.x() - 1][p.y()] = true;
			}
			if(checkFillValidity(canvas, p.x(), p.y() + 1, targetColour, tolerance, filled)) {
				q.enqueue(QPoint(p.x(), p.y() + 1));
				filled[p.x()][p.y() + 1] = true;
			}
			if(checkFillValidity(canvas, p.x(), p.y() - 1, targetColour, tolerance, filled)) {
				q.enqueue(QPoint(p.x(), p.y() - 1));
				filled[p.x() ][p.y() - 1] = true;
			}
			if(settings.fill8Way) {
				if(checkFillValidity(canvas, p.x() + 1, p.y() + 1, targetColour, tolerance, filled)) {
					q.enqueue(QPoint(p.x() + 1, p.y() + 1));
					filled[p.x() + 1][p.y() + 1] = true;
				}
				if(checkFillValidity(canvas, p.x() - 1, p.y() - 1, targetColour, tolerance, filled)) {
					q.enqueue(QPoint(p.x() - 1, p.y() - 1));
					filled[p.x() - 1][p.y() - 1] = true;
				}
				if(checkFillValidity(canvas, p.x() - 1, p.y() + 1, targetColour, tolerance, filled)) {
					q.enqueue(QPoint(p.x() - 1, p.y() + 1));
					filled[p.x() - 1][p.y() + 1] = true;
				}
				if(checkFillValidity(canvas, p.x() + 1, p.y() - 1, targetColour, tolerance, filled)) {
					q.enqueue(QPoint(p.x() + 1, p.y() - 1));
					filled[p.x() + 1][p.y() - 1] = true;
				}
			}
		}
	}

	// Maybe taking the raw data could speed this up
	bool Algorithms::checkFillValidity(Canvas& canvas, int i, int j, uint targetColour, int tolerance, bool** filled) {
		if(i >= 0 && i < canvas.buffer->width() && j >= 0 && j < canvas.buffer->height()) {
			// Note that the QImage.pixel function is expensive when used lots. Maybe use QImage.bits for fast access to raw data?
			uint canvasCol = canvas.buffer->pixel(i, j);
			if(canFill(targetColour, canvasCol, tolerance) || (Colour::getAlpha(canvasCol) == 0 && Colour::getAlpha(targetColour == 0))) {
				if(!filled[i][j]) {
					return true;
				}
			}
		}
		return false;
	}

	bool Algorithms::canFill(uint targetColour, uint colour, int tolerance) {
		ushort diff = Colour::getDifference(targetColour, colour, true);
		return utils::map(diff, 0, 510, 0, 255) <= tolerance;
	}
}