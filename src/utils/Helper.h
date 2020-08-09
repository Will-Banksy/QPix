#ifndef HELPER_H
#define HELPER_H

#include <stdlib.h>
#include <QString>

namespace utils {
	// char* addstrs(char* str1, char* str2, int str1len, int str2len);
	float map(float val, float start1, float stop1, float start2, float stop2);
	QString getStyleSheet();
}

#endif // HELPER_H