#include "PaintUtils.h"
#include <QImage>
#include <QStack>
#include "Utils.h"
#include <iostream>
#include <QQueue>

uint8_t* PaintUtils::s_ExtraDrawData = nullptr;
QSize PaintUtils::s_ExtraDrawDataSize = QSize();

void PaintUtils::reset(const QSize& surfaceSize) {
	if(surfaceSize != s_ExtraDrawDataSize) {
		s_ExtraDrawData = new uint8_t[surfaceSize.width() * surfaceSize.height()];
		s_ExtraDrawDataSize = surfaceSize;
	}
	memset(s_ExtraDrawData, 0, s_ExtraDrawDataSize.width() * s_ExtraDrawDataSize.height());
}

void PaintUtils::drawLine(QImage& target, int x0, int y0, int x1, int y1, QRgb colour, QList<QPoint>* stroke, bool recordFirst) {
	QRgb* bytes = (QRgb*)target.scanLine(0); // Assumes a 32-bit format

	int startX = x0;
	int startY = y0;

	int dx = abs(x1 - x0);
	int dy = -abs(y1 - y0);

	int unitX = x0 < x1 ? 1 : -1;
	int unitY = y0 < y1 ? 1 : -1;

	int err = dx + dy;
	int err2;

	for(;;) {
		// Check that the point is in drawable space first
		bool inDrawableSpace = utils::contains(target, QPoint(x0, y0));
		if(inDrawableSpace) {
			bytes[x0 + y0 * target.width()] = colour;
		}

		// Record the point in stroke and the extra draw data unless recordFirst is false and this is the starting pixel
		if(recordFirst || startX != x0 || startY != y0) {
			// But being in drawable space doesn't matter for recording the stroke - We want to record the stroke out of drawable space
			if(stroke != nullptr) {
				stroke->emplace_back(x0, y0);
			}

			if(inDrawableSpace) {
				// Record a point being put here
				s_ExtraDrawData[x0 + y0 * target.width()] += 1;
			}
		}

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

void PaintUtils::affectLine(QImage& target, int x0, int y0, int x1, int y1, LineAffector f, bool oncePerPixel) {
	QRgb* bytes = (QRgb*)target.scanLine(0); // Assumes a 32-bit format

	int dx = abs(x1 - x0);
	int dy = -abs(y1 - y0);

	int unitX = x0 < x1 ? 1 : -1;
	int unitY = y0 < y1 ? 1 : -1;

	int err = dx + dy;
	int err2;

	for(;;) {
		// Check that the point is in drawable space first
		bool inDrawableSpace = utils::contains(target, QPoint(x0, y0));

		int idx = x0 + y0 * target.width();
		bool visited = s_ExtraDrawData[idx] > 0;
		if(inDrawableSpace && (!visited || !oncePerPixel)) {
			bytes[idx] = f(bytes[idx]);
			s_ExtraDrawData[idx] += 1;
		}

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

void PaintUtils::pixelPerfectCorrect(const QImage& surface, QImage& buffer, QList<QPoint>& stroke) {
	if(stroke.length() < 3) {
		return;
	}

	QRgb* bufdat = (uint32_t*)buffer.scanLine(0); // Assumes a 32-bit format
	QRgb* surfdat = (uint32_t*)surface.scanLine(0); // Assumes a 32-bit format

	qDebug() << "Pixel perfect correct";

	for(int i = 1; i < stroke.length() - 1; i++) {
		if(stroke.length() >= 3 && utils::contains(buffer, stroke.at(i))) {
			QPoint pts[3] = {
				stroke.at(i - 1), stroke.at(i), stroke.at(i + 1)
			};

			// Detects an L shape, with pts[1] being the corner pixel, e.g.:
			// 0
			// 1 2
			if(
				   (pts[0].x() == pts[1].x() || pts[0].y() == pts[1].y())
				&& (pts[1].x() == pts[2].x() || pts[1].y() == pts[2].y())
				&& (pts[0].x() != pts[2].x())
				&& (pts[0].y() != pts[2].y())
			) {
				// Then removes the corner pixel from the list
				stroke.removeAt(i);
				i--;

				// Then checks if this is the only point that has filled in the pixel - Which will
				// correspond to the number recorded in the s_ExtraDrawData array at the corresponding index
				// being 0 after decrement
				size_t idx = pts[1].x() + pts[1].y() * buffer.width();
				s_ExtraDrawData[idx] -= 1;
				if(s_ExtraDrawData[idx] == 0) {
					bufdat[idx] = surfdat[idx];
				}
			}
		}
	}

	// Remove all points except the last 2
	if(stroke.length() > 2) {
		stroke.remove(0, stroke.length() - 3);
	}

	// Old code
	//
	// for(int i = 1; i < path.count() - 1; i++) {
	// 	if(((path[i - 1].x() == path[i].x() || path[i - 1].y() == path[i].y())
	// 			&& (path[i + 1].x() == path[i].x() || path[i+1].y() == path[i].y())
	// 			&&  path[i - 1].x() != path[i + 1].x()
	// 			&&  path[i - 1].y() != path[i + 1].y())) {
	// 		brush.applyBrush(path[i].x(), path[i].y(), bytes, target.size(), currentStroke.at(i).second);//target.setPixel(path[i].x(), path[i].y(), currentStroke.at(i).second);
	// 		currentStroke.removeAt(i);
	// 	}
	// }
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
	stack.emplace_back(x, y);

	PaintUtils::reset(target.size());

	while(!stack.isEmpty()) {
		QPoint pt = stack.pop();

		x = pt.x();
		y = pt.y();

		bytes[x + y * target.width()] = colour;

		PaintUtils::fillPixel(target, bytes, x, y - 1, (bool*)s_ExtraDrawData, stack, origCol, colour, tolerance);
		PaintUtils::fillPixel(target, bytes, x, y + 1, (bool*)s_ExtraDrawData, stack, origCol, colour, tolerance);
		PaintUtils::fillPixel(target, bytes, x - 1, y, (bool*)s_ExtraDrawData, stack, origCol, colour, tolerance);
		PaintUtils::fillPixel(target, bytes, x + 1, y, (bool*)s_ExtraDrawData, stack, origCol, colour, tolerance);

		if(fillDiagonally) {
			PaintUtils::fillPixel(target, bytes, x - 1, y - 1, (bool*)s_ExtraDrawData, stack, origCol, colour, tolerance);
			PaintUtils::fillPixel(target, bytes, x + 1, y + 1, (bool*)s_ExtraDrawData, stack, origCol, colour, tolerance);
			PaintUtils::fillPixel(target, bytes, x - 1, y + 1, (bool*)s_ExtraDrawData, stack, origCol, colour, tolerance);
			PaintUtils::fillPixel(target, bytes, x + 1, y - 1, (bool*)s_ExtraDrawData, stack, origCol, colour, tolerance);
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
