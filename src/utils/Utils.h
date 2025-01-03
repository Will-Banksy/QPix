#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <QString>
#include <QRect>
#include <QPalette>
#include "ui/components/floating/FloatingView.h"

class QImage;
class QPoint;
class QWidget;

namespace utils {
	/// Loads the colour palette from the src path, and copies and modifies basePalette with the colours defined in the src file in JSON
	/// format, returning the modified QPalette
	QPalette loadPaletteFrom(const QString& src, QPalette& basePalette);

	/// Creates a copy of inner that is adjusted to lie within outer
	QRect adjustedToWithin(const QRect& outer, const QRect& inner);

	/// Calculates the area of a QRect
	int area(const QRect& rect);

	/// Calculates the best position for self to be positioned relative src, according to the "position" enum.
	/// Returns the top-left corner of the rectangle that should be self's new geometry.
	QPoint repositionFloating(const QWidget* self, const QWidget* src, FloatingPosition position);

	template<typename T>
	T constrain(T num, T min, T max) {
		if(num < min) {
			return min;
		} else if(num > max) {
			return max;
		}
		return num;
	}

	/// Maps a value that is between start1 and stop1 to between start2 and stop2
	float map(float val, float start1, float stop1, float start2, float stop2);

	// Below is old functions that are unused by new QPix

	// char* addstrs(char* str1, char* str2, int str1len, int str2len);
	QString getStyleSheet();
	uint32_t* colAt(uint32_t* argbArr, int width, int i, int j);
	QRect expandRound(const QRectF& rect);

	/// Where bitPos is the index of the byte counting from the right hand side of the byte
	void setBit(quint8& byte, quint8 bitPos, bool val);

	/// Where bitPos is the index of the byte counting from the right hand side of the byte
	bool getBit(const quint8& byte, quint8 bitPos);

	/// Necessary because C++ uses the dumb modulo, like Java and C#, and unlike Python, surprisingly
	int mod(int divisor, int dividend);

	/// Simply returns a list of the points in a line from start to end, using Bresenham's line algorithm
	QList<QPoint> plotLine(int startX, int startY, int endX, int endY);

	/// Returns true if the image bounds contains the point
	bool contains(const QImage& image, const QPoint& point);

	// Can't split templates across header and source files: https://stackoverflow.com/questions/5612791/c-template-and-header-files#5612847
	// Reason is that the WHOLE function/class/thing that is templated needs to be known at compile time, so it can actually generate the function/class/thing according to it's usage
	// And of course, definitions in header files are matched to their implementations (if elsewhere) at linking time, which is too late
	template<typename T>
	T** create2dArray(int width, int height, T fillWith = 0) {
		T** arr = new T*[width];

		for(int i = 0; i < width; i++) {
			arr[i] = new T[height];
			for(int j = 0; j < height; j++) {
				arr[i][j] = fillWith;
			}
		}
		return arr;
	}
}

#endif // UTILS_H