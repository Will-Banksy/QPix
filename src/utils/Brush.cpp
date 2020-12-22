#include "Brush.h"
#include "EditorTools.h"
#include "Helper.h"

namespace utils {
	void Brush::applyBrush(int i, int j, uint* colArray, QSize imageSize, uint col) const {
		std::cout << "incorrect applyBrush" << std::endl;
	}

	void VariableBrush::applyBrush(int i, int j, uint* colArray, QSize imageSize, uint col) const {
// 		std::cout << "applyBrush" << std::endl;
		if(EditorTools::brushWidth == 1 && EditorTools::brushHeight == 1) {
			// TODO Implement the ability to 'tile' your strokes
			bool tileBrush = true;
			if(i >= 0 && i < imageSize.width() && j >= 0 && j < imageSize.height()) {
				int index = i + j * imageSize.width();
				colArray[index] = col;
			} else if(tileBrush) {
				// Working amazingly thanks to a different definition of modulo
				i = utils::mod(i, imageSize.width());
				j = utils::mod(j, imageSize.height());
				if(i >= 0 && i < imageSize.width() && j >= 0 && j < imageSize.height()) { // Just in case
					int index = i + j * imageSize.width();
					colArray[index] = col;
				}
			}
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