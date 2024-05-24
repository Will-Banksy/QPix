#ifndef PAINTUTILS_H
#define PAINTUTILS_H

#include <cstdint>
#include <QRgb>
#include <QPoint>
#include <QSize>

class QImage;

/// Basically just a copy of Painter but slightly changed to be better and fit in with the different way
/// we're doing things now
class PaintUtils {
public:
	// TODO: Add brushes

	/// Utilises the Bresenham line algorithm to draw a line from (x0, y0) to (x1, y1) in the supplied colour
	static void drawLine(QImage& target, int x0, int y0, int x1, int y1, QRgb colour);

	/// Flood-fills an area of pixels starting from (x, y), flooding to adjacent pixels if the difference between
	/// the adjacent pixel is within the tolerance of the starting pixel colour
	static void fillArea(QImage& target, int x, int y, QRgb colour, int tolerance);

private:
	explicit PaintUtils() = delete;
	~PaintUtils() = delete;

	static inline void fillPixel(const QImage& target, QRgb* bytes, int x, int y, bool* filled, QStack<QPoint>& stack, QRgb origCol, QRgb colour, int tolerance);
	static bool canFill(QRgb targetColour, QRgb origColour, int tolerance);

	// TODO: Is this necessary? It's the memset that's the actual performance issue rather than the allocation
	static bool* s_CachedFillArray;
	static QSize s_CachedFillArraySize;
};

#endif // PAINTUTILS_H