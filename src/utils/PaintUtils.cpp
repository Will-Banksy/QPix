#include "PaintUtils.h"
#include <QImage>

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