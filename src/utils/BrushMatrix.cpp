#include "BrushMatrix.h"
#include <sys/types.h>
#include <stdlib.h>

namespace utils {
	inline uint ceil(float val) {
		return static_cast<uint>(val) + 1; // Don't know if I need to static_cast, maybe I can just (uint) ?
	}

	BrushMatrix::BrushMatrix(ushort width, ushort height, QList<QPoint> points) {
		matrix = makePackedMatrix(width, height);
	}

	BrushMatrix::~BrushMatrix() {
		delete[] matrix;
	}

	char* BrushMatrix::makePackedMatrix(ushort width, ushort height) {
		uint numBytes = ceil((width * height) / 8);
		char* matrix = new char[numBytes];//(char*)malloc(numBytes);
		// TODO: Complete implementation of class: I'm trying to store boolean bits in the most memory-efficient way I can, in preparation for large brushes. I want this app to be lightweight. Plus it's trying something new.
	}
}