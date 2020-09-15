#include "Brush.h"
#include "EditorTools.h"

namespace utils {
	void Brush::applyBrush(int i, int j, uint* colArray, QSize imageSize, uint col) const {
		std::cout << "incorrect applyBrush" << std::endl;
	}

	void VariableBrush::applyBrush(int i, int j, uint* colArray, QSize imageSize, uint col) const {
		std::cout << "applyBrush" << std::endl;
		if(EditorTools::brushWidth == 1 && EditorTools::brushHeight == 1) {
			int index = i + j * imageSize.width();
			colArray[index] = col;
		}
	}

	void FixedBrush::applyBrush(int i_, int j_, uint* colArray, QSize imageSize, uint col) const {
		uchar startX = i_ - m_brushMatrix.width() / 2;
		uchar startY = j_ - m_brushMatrix.height() / 2;

		for(uint i = startX; i < imageSize.width(); i++) {
			for(uint j = startY; j < imageSize.height(); j++) {
				ushort matrixI = i - startX;
				ushort matrixJ = j - startY;
				bool colourThis = m_brushMatrix.at(matrixI, matrixJ);
				if(colourThis) {
					int index = i + j * imageSize.width();
					colArray[index] = col;
				}
			}
		}
	}
}