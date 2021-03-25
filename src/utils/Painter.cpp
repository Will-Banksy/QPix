#include "Painter.h"
#include <QQueue>
#include <QPoint>
#include <QPointF>
#include "Helper.h"
#include "Colour.h"
#include <cmath>
#include "EditorTools.h"

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

	void Painter::drawLine(QImage& target, int startX, int startY, int endX, int endY, uint col, const Brush& brush) {
		int& x0 = startX;
		int& y0 = startY;
		int& x1 = endX;
		int& y1 = endY;

		int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
		int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
		int err = dx+dy, e2; /* error value e_xy */

		uint* bytes = (uint*)target.bits();

		for(;;){  /* loop */
			// Call the function with the () operator
// 			action(x0, y0, col, settings);
			brush.applyBrush(x0, y0, bytes, target.size(), col);
			if (x0==x1 && y0==y1) break;
			e2 = 2*err;
			if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
				if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
		}
	}

	void Painter::drawUniformLine(QImage& target, int startX, int startY, int endX, int endY, uint col, const Brush& brush) {
		uint* bytes = (uint*)target.bits();

		int& x0 = startX;
		int& y0 = startY;
		int& x1 = endX;
		int& y1 = endY;

		float dx = abs(x1 - x0) + 1;
		float dy = abs(y1 - y0) + 1;

		float sx = (x0 < x1) ? 1 : -1;
		float sy = (y0 < y1) ? 1 : -1;

		float ratio = fmax(dx, dy) / fmin(dx, dy);

		int pixelStep = round(ratio);

		if (pixelStep > fmin(dx, dy)) {
			pixelStep = (int)std::numeric_limits<double>::infinity();//(int)Double.POSITIVE_INFINITY;
		}

		float maxDistance = (float)distance(x0, y0, x1, y1);

		int x = x0;
		int y = y0;
		int i = 0;
		while (true) {
			i++;

// 			action(x, y, col, settings);
			brush.applyBrush(x, y, bytes, target.size(), col);
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

	void Painter::drawLinePixelPerfect(QImage& target, int startX, int startY, int endX, int endY, uint col, const Brush& brush, QList<QPair<QPoint, uint>>& currentStroke) {
		uint* bytes = (uint*)target.bits();

		if((EditorTools::brushWidth != 1) || (EditorTools::brushHeight != 1)) {
			drawLine(target, startX, startY, endX, endY, col, brush);
			return;
		}

		QList<QPoint> newLine = utils::plotLine(startX, startY, endX, endY);

		QPoint* prevPoint = nullptr;
		if(!currentStroke.isEmpty()) {
			prevPoint = &currentStroke[currentStroke.count() - 1].first;
		}

		for(int i = 0; i < newLine.count(); i++) {
			if(prevPoint) {
				if(*prevPoint == newLine.at(i)) {
					continue;
				}
			}
			currentStroke.append(QPair<QPoint, uint>(newLine[i], getPixel(target, newLine[i].x(), newLine[i].y())));
			brush.applyBrush(newLine[i].x(), newLine[i].y(), bytes, target.size(), col);
			prevPoint = &newLine[i];
		}

		QList<QPoint> path;
		for(int i = 0; i < currentStroke.count(); i++) {
			path.append(currentStroke.at(i).first);
		}

		for(int i = 1; i < path.count() - 1; i++) {
			if(((path[i - 1].x() == path[i].x() || path[i - 1].y() == path[i].y())
					&& (path[i + 1].x() == path[i].x() || path[i+1].y() == path[i].y())
					&&  path[i - 1].x() != path[i + 1].x()
					&&  path[i - 1].y() != path[i + 1].y())) {
				brush.applyBrush(path[i].x(), path[i].y(), bytes, target.size(), currentStroke.at(i).second);//target.setPixel(path[i].x(), path[i].y(), currentStroke.at(i).second);
				currentStroke.removeAt(i);
			}
		}
	}

	// I would say this is fast enough, for now
	void Painter::fillArea(QImage& target, int startX, int startY, uint col, const Brush& brush, int tolerance, bool fill8Way) {
		startX = utils::mod(startX, target.width());
		startY = utils::mod(startY, target.height());

		bool** filled = utils::create2DArray<bool>(target.width(), target.height(), false);

		uint targetColour = target.pixel(startX, startY);

		uint* bytes = (uint*)target.bits(); // Get the bytes for fast writing/reading. The bytes are in order: A, R, G, B, A, R, G, B, ... so I can just collect them as a uint

		QQueue<QPoint> q;

		// enqueue(...) to push, dequeue() to pop
		q.enqueue(QPoint(startX, startY));

		while(!q.isEmpty()) {
			QPoint p = q.dequeue();

// 			*colAt(bytes, canvas.buffer->width(), p.x(), p.y()) = col; // This should be roughly equivalent to canvas.buffer->setPixel(i, j, col)
// 			action(p.x(), p.y(), col, settings);
			brush.applyBrush(p.x(), p.y(), bytes, target.size(), col);

			if(checkFillValidity(target, p.x() + 1, p.y(), targetColour, tolerance, filled)) {
				q.enqueue(QPoint(p.x() + 1, p.y()));
				filled[p.x() + 1][p.y()] = true;
			}
			if(checkFillValidity(target, p.x() - 1, p.y(), targetColour, tolerance, filled)) {
				q.enqueue(QPoint(p.x() - 1, p.y()));
				filled[p.x() - 1][p.y()] = true;
			}
			if(checkFillValidity(target, p.x(), p.y() + 1, targetColour, tolerance, filled)) {
				q.enqueue(QPoint(p.x(), p.y() + 1));
				filled[p.x()][p.y() + 1] = true;
			}
			if(checkFillValidity(target, p.x(), p.y() - 1, targetColour, tolerance, filled)) {
				q.enqueue(QPoint(p.x(), p.y() - 1));
				filled[p.x() ][p.y() - 1] = true;
			}
			if(fill8Way) {
				if(checkFillValidity(target, p.x() + 1, p.y() + 1, targetColour, tolerance, filled)) {
					q.enqueue(QPoint(p.x() + 1, p.y() + 1));
					filled[p.x() + 1][p.y() + 1] = true;
				}
				if(checkFillValidity(target, p.x() - 1, p.y() - 1, targetColour, tolerance, filled)) {
					q.enqueue(QPoint(p.x() - 1, p.y() - 1));
					filled[p.x() - 1][p.y() - 1] = true;
				}
				if(checkFillValidity(target, p.x() - 1, p.y() + 1, targetColour, tolerance, filled)) {
					q.enqueue(QPoint(p.x() - 1, p.y() + 1));
					filled[p.x() - 1][p.y() + 1] = true;
				}
				if(checkFillValidity(target, p.x() + 1, p.y() - 1, targetColour, tolerance, filled)) {
					q.enqueue(QPoint(p.x() + 1, p.y() - 1));
					filled[p.x() + 1][p.y() - 1] = true;
				}
			}
		}
	}

	// Maybe taking the raw data could speed this up
	bool Painter::checkFillValidity(QImage& target, int i, int j, uint targetColour, int tolerance, bool** filled) {
		if(i >= 0 && i < target.width() && j >= 0 && j < target.height()) {
			// Note that the QImage.pixel function is expensive when used lots. Maybe use QImage.bits for fast access to raw data?
			uint canvasCol = target.pixel(i, j);
			if(canFill(targetColour, canvasCol, tolerance) || (Colour::getAlpha(canvasCol) == 0 && Colour::getAlpha(targetColour == 0))) {
				if(!filled[i][j]) {
					return true;
				}
			}
		}
		return false;
	}

	bool Painter::canFill(uint targetColour, uint colour, int tolerance) {
		ushort diff = Colour::getDifference(targetColour, colour, true);
		return utils::map(diff, 0, 510, 0, 255) <= tolerance;
	}

	uint Painter::getPixel(QImage& target, int i, int j) {
		i = utils::mod(i, target.width());
		j = utils::mod(j, target.height());
		return target.pixel(i, j);
	}
}