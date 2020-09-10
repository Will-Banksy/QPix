#ifndef HELPER_H
#define HELPER_H

#include <stdlib.h>
#include <QString>
#include <QRect>

namespace utils {
	// char* addstrs(char* str1, char* str2, int str1len, int str2len);
	float map(float val, float start1, float stop1, float start2, float stop2);
	QString getStyleSheet();
	uint* colAt(uint* argbArr, int width, int i, int j);
	QRect expandRound(const QRectF& rect);
	/// Where bitPos is the index of the byte counting from the right hand side of the byte
	void setBit(uchar& byte, uchar bitPos, bool val);
	/// Where bitPos is the index of the byte counting from the right hand side of the byte
	bool getBit(const uchar& byte, uchar bitPos);

	// Can't split templates across header and source files: https://stackoverflow.com/questions/5612791/c-template-and-header-files#5612847
	// Reason is that the WHOLE function/class/thing that is templated needs to be known at compile time, so it can actually generate the function/class/thing according to it's usage
	// And of course, definitions in header files are matched to their implementations (if elsewhere) at linking time, which is too late
	template<typename T>
	T** create2DArray(int width, int height, T fillWith = 0) {
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

#endif // HELPER_H