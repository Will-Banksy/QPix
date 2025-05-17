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

	/// Resets internal state used by some drawing functions (e.g. drawLine, pixelPerfectCorrect). Should be called
	/// before a new "operation" (however that may be decided) in order to ensure correct behaviour
	/// Requires a size that must correspond to the size of the image of the current surface image
	static void reset(const QSize& surfaceSize);

	/// Utilises the Bresenham line algorithm to draw a line from (x0, y0) to (x1, y1) in the supplied colour.
	/// If stroke is provided, then this function will record points drawn into stroke. If recordFirst is false, then
	/// the first point will not be recorded
	static void drawLine(QImage& target, int x0, int y0, int x1, int y1, QRgb colour, QList<QPoint>* stroke = nullptr, bool recordFirst = true);

	/// Utilises my own algorithm to draw a uniform line from (x0, y0) to (x1, y1) in the supplied colour. A uniform line
	/// is one that is composed of equal sized segments, or more mathematically where the `width / height` of the line
	/// is an integer.
	/// This function actually doesn't always draw perfectly uniform lines - It will draw a uniform line from (x0, y0)
	/// until it reaches x1 or y1, at which point it will stop - Possibly resulting in less pixels in the last segment
	/// than the rest.
	/// Specify maxSegmentSize to limit the length of each segment, or <= 0 for unlimited
	static void drawUniformLine(QImage& target, int x0, int y0, int x1, int y1, QRgb colour, int maxSegmentSize = 0);

	/// Utilises the Bresenham line algorithm to copy a line from (x0, y0) to (x1, y1), sourcing pixels from `src`
	/// and copying them to `dest`
	static void copyLine(const QImage& src, QImage& dest, int x0, int y0, int x1, int y1);

	/// Utilises my own algorithm to copy a uniform line from (x0, y0) to (x1, y1), sourcing pixels from `src` and
	/// copying them to `dest`. For more info on uniform lines, see `drawUniformLine`
	static void copyUniformLine(const QImage& src, QImage& dest, int x0, int y0, int x1, int y1, int maxSegmentSize = 0);

	/// Alias for a function taking and returning a QRgb
	using LineAffector = std::function<QRgb(QRgb)>;

	/// Runs `f` on every pixel of a line, using bresenham's line algorithm. Supply true for oncePerPixel to make the function only run once per pixel
	/// between PaintUtils state resets
	static void affectLine(QImage& target, int x0, int y0, int x1, int y1, LineAffector f, bool oncePerPixel = false);

	/// Applies pixel perfect corrections to a list of points that make up a stroke, assuming that the stroke is already painted
	/// on to buffer but not on to surface (i.e. surface holds the state just before the stroke). Removes points that are no longer
	/// needed for pixel perfect corrections from stroke
	static void pixelPerfectCorrect(const QImage& surface, QImage& buffer, QList<QPoint>& stroke);

	/// Flood-fills an area of pixels starting from (x, y), flooding to adjacent pixels if the difference between
	/// the adjacent pixel is within the tolerance of the starting pixel colour, according to `PaintUtils::canFill`
	static void fillArea(QImage& target, int x, int y, QRgb colour, int tolerance, bool fillDiagonally);

private:
	explicit PaintUtils() = delete;
	~PaintUtils() = delete;

	static inline void fillPixel(const QImage& target, QRgb* bytes, int x, int y, bool* filled, QStack<QPoint>& stack, QRgb origCol, QRgb colour, int tolerance);
	static bool canFill(QRgb targetColour, QRgb origColour, int tolerance);

	static uint8_t* s_ExtraDrawData;
	static QSize s_ExtraDrawDataSize;
};

#endif // PAINTUTILS_H