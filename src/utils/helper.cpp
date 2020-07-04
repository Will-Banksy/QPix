#include "helper.h"
#include <cstring>

// using namespace utils;

/**
 * Uncertain if this actually works - should in theory
 */
// char* addstrs(char* str1, char* str2, int str1len, int str2len) {
// 	char* newarr = (char*)malloc(str1len + str2len);
// 	memcpy((void*)newarr, (void*)str1, str1len);
// 	memcpy((void*)&newarr[str1len], (void*)str2, str2len);
// 	return newarr;
// }

float utils::map(float val, float start1, float stop1, float start2, float stop2) {
	return start2 + (stop2 - start2) * ((val - start1) / (stop1 - start1));
}