#ifndef PAINTUTILS_H
#define PAINTUTILS_H

#include <cstdint>
#include <QRgb>
#include <QPoint>
#include <QSize>

class QImage;

// NOTE: Thoughts for brushes... So we have the following ways of defining brushes:
//       - Fragment Shader - Or really just a little function that outputs whether to colour a pixel or not (or the colour to colour a pixel...) based on a UV coordinate (so is automatically scaled)
//       - Shape - Define a regular polygon or circle with some transformations perhaps that is automatically sized and used to paint with the selected colour
//       - Bitmap - Define a grid of colours to colour pixels, or a grid of what pixels should be coloured. Can only be integer scaled
//       I think it'd be nice to have a way for each brush type to paint with a set colour or the currently selected colour, or even to define which pixels should be painted with primary and which with secondary
//       We should be able to allow users to define Shape and Bitmap brushes in the UI... (Bitmap might be more complex to implement) but for Fragment Shader brushes maybe lua scripts?
//       There will have to be a lot of work gone in to optimization most likely, especially if we go with lua scripts
//           Well actually for Frag Shader and Shape brushes we can cache the current size as a bitmap and optimize that - So never actually paint directly with those
enum class BrushType {
	FragmentShader,
	Shape,
	Bitmap
};

/// Basically just a copy of Painter but slightly changed to be better and fit in with the different way
/// we're doing things now
class PaintUtils {
public:
	// TODO: Add brushes

	/// Utilises the Bresenham line algorithm to draw a line from (x0, y0) to (x1, y1) in the supplied colour
	static void drawLine(QImage& target, int x0, int y0, int x1, int y1, QRgb colour);

	/// Flood-fills an area of pixels starting from (x, y), flooding to adjacent pixels if the difference between
	/// the adjacent pixel is within the tolerance of the starting pixel colour
	static void fillArea(QImage& target, int x, int y, QRgb colour, int tolerance, bool fillDiagonally);

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