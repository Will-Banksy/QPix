#include "PaintUtils.h"
#include <QImage>
#include <QStack>
#include "Utils.h"
#include <iostream>

bool* PaintUtils::s_CachedFillArray = nullptr;
QSize PaintUtils::s_CachedFillArraySize = QSize();

void PaintUtils::drawLine(QImage& target, int x0, int y0, int x1, int y1, QRgb colour) {
	QRgb* bytes = (uint32_t*)target.scanLine(0); // Assumes a 32-bit format

	int dx = abs(x1 - x0);
	int dy = -abs(y1 - y0);

	int unitX = x0 < x1 ? 1 : -1;
	int unitY = y0 < y1 ? 1 : -1;

	int err = dx + dy;
	int err2;

	for(;;) {
		bytes[x0 + y0 * target.width()] = colour;

		if(x0 == x1 && y0 == y1) {
			break;
		}
		err2 = err * 2;
		if(err2 >= dy) {
			err += dy;
			x0 += unitX;
		}
		if(err2 <= dx) {
			err += dx;
			y0 += unitY;
		}
	}
}

void PaintUtils::fillArea(QImage& target, int x, int y, QRgb colour, int tolerance, bool fillDiagonally) {
	// How best to implement a flood fill algorithm? My approach with a stack performs decently enough
	//
	// Sources:
	// - https://www.youtube.com/watch?v=LvacRISl99Y
	// - https://www.codeproject.com/Articles/5312368/Five-Fast-Flood-Fills

	if(x < 0 || y < 0 || x >= target.width() || y >= target.height()) {
		return;
	}

	QRgb* bytes = (QRgb*)target.scanLine(0); // Assumes a 32-bit format

	QRgb origCol = bytes[x + y * target.width()];

	QStack<QPoint> stack = QStack<QPoint>();
	// Reserve some memory for the stack to avoid too many allocations
	stack.reserve((target.width() * target.height()) / 2);
	stack.emplace_back(QPoint(x, y));

	if(s_CachedFillArraySize != target.size()) {
		s_CachedFillArray = new bool[target.width() * target.height()];
		s_CachedFillArraySize = target.size();
	}

	memset(s_CachedFillArray, false, target.width() * target.height());

	while(!stack.isEmpty()) {
		QPoint pt = stack.pop();

		x = pt.x();
		y = pt.y();

		bytes[x + y * target.width()] = colour;

		PaintUtils::fillPixel(target, bytes, x, y - 1, s_CachedFillArray, stack, origCol, colour, tolerance);
		PaintUtils::fillPixel(target, bytes, x, y + 1, s_CachedFillArray, stack, origCol, colour, tolerance);
		PaintUtils::fillPixel(target, bytes, x - 1, y, s_CachedFillArray, stack, origCol, colour, tolerance);
		PaintUtils::fillPixel(target, bytes, x + 1, y, s_CachedFillArray, stack, origCol, colour, tolerance);

		if(fillDiagonally) {
			PaintUtils::fillPixel(target, bytes, x - 1, y - 1, s_CachedFillArray, stack, origCol, colour, tolerance);
			PaintUtils::fillPixel(target, bytes, x + 1, y + 1, s_CachedFillArray, stack, origCol, colour, tolerance);
			PaintUtils::fillPixel(target, bytes, x - 1, y + 1, s_CachedFillArray, stack, origCol, colour, tolerance);
			PaintUtils::fillPixel(target, bytes, x + 1, y - 1, s_CachedFillArray, stack, origCol, colour, tolerance);
		}
	}
}

inline void PaintUtils::fillPixel(const QImage& target, QRgb* bytes, int x, int y, bool* filled, QStack<QPoint>& stack, QRgb origCol, QRgb colour, int tolerance) {
	if(x < 0 || y < 0 || x >= target.width() || y >= target.height()) {
		return;
	}

	int i = x + y * target.width();

	QRgb& targetPixel = bytes[i];

	if(!filled[i] && PaintUtils::canFill(targetPixel, origCol, tolerance)) {
		stack.emplace_back(QPoint(x, y));
		filled[i] = true;
	}
}

bool PaintUtils::canFill(QRgb targetColour, QRgb origColour, int tolerance) {
	return targetColour == origColour; // FIXME: Ignoring tolerance for now
}
