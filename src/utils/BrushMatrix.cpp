#include "BrushMatrix.h"
#include <sys/types.h>
#include "Helper.h"

namespace utils {
	inline uint ceil(float val) {
		return (uint)(val) + 1; // Don't think I need to static_cast here, I think that's for pointers
	}

	BrushMatrix::BrushMatrix() : BrushMatrix(0, 0, {}) {
	}

	BrushMatrix::BrushMatrix(ushort width, ushort height, const QList<QPoint>& points) {
		matrix = makePackedMatrix(width, height);
		for(QPoint p : points) {
			setAt(matrix, p.x(), p.y(), 1u);
		}
	}

	BrushMatrix::~BrushMatrix() {
		delete[] matrix;
	}

	uchar* BrushMatrix::makePackedMatrix(ushort width, ushort height) { // Well, not really a matrix, really an array
		this->width = width;
		this->height = height;
		uint numBytes = ceil((width * height) / 8);
		uchar* matrix = new uchar[numBytes];//(char*)malloc(numBytes);
		for(int i = 0; i < numBytes; i++) {
			matrix[i] = 0; // Populate the array with 0
		}
		return matrix;
	}

	bool BrushMatrix::at(int i, int j) {
		if(i < 0 || i > (width * height / 8)) {
			return false; // Out of bounds, so return false
		}
		uint bit = i + j * width; // The bit that we need to get (offset from start of array)
		uint byte = bit / 8;/*(uint)((float)bit / 8.0);*/ // The byte that contains the bit we need to get
		uint bytebit = bit - (byte * 8); // The bit we need to get (offset from the start of the byte)

		uchar& byteVal = matrix[byte];
		bool val = utils::getBit(byteVal, bytebit);
		return val;
	}

	void BrushMatrix::setAt(uchar* matrix, int i, int j, bool val) {
		if(i < 0 || i > (width * height / 8)) {
			return; // Out of bounds, so return
		}
		uint bit = i + j * width; // The bit that we need to set
		uint byte = bit / 8;/*(uint)((float)bit / 8.0);*/ // The byte that contains the bit we need to change
		uint bytebit = 8 - (bit - (byte * 8)); // The bit we need to get (offset from the start (rightmost end) of the byte)

		uchar& byteVal = matrix[byte];
		utils::setBit(byteVal, bytebit, val);
	}
}