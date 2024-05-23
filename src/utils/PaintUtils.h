#ifndef PAINTUTILS_H
#define PAINTUTILS_H

#include <cstdint>
#include <QRgb>

class QImage;

class PaintUtils {
public:
	// TODO: Add brushes

	static void drawLine(QImage& target, int x0, int y0, int x1, int y1, QRgb colour);

private:
	explicit PaintUtils() = delete;
	~PaintUtils() = delete;
};

#endif // PAINTUTILS_H