#ifndef BRUSH_H
#define BRUSH_H

#include "BrushMatrix.h"
#include <QSize>
#include <iostream>

/**
 * @todo write docs
 */
namespace utils {
	enum BrushType {
		/// Variable brushes are defined by brush shapes, and can be used at any size
		VARIABLE,
		/// Fixed brushes are defined by a matrix, and can only be used at the size of the matrix
		FIXED,
		/// No brush
		NONE
	};

	enum BrushShape {
		RECT,
		ELLIPSE
	};

	class Brush {
		public:
			Brush(BrushType type = NONE) : m_type(type) {}
			virtual void applyBrush(int i_, int j_, uint* colArray, QSize imageSize, uint col) const; // const - won't change members

			const BrushType m_type; // m_ is a convention that means m_{var} var is a member, there's others like s_ for static I think
	};

	class VariableBrush : public Brush {
		public:
			VariableBrush(BrushShape shape = RECT) : Brush(VARIABLE), m_shape(shape) {};
			void applyBrush(int i_, int j_, uint* colArray, QSize imageSize, uint col) const override;

			const BrushShape m_shape;
	};

	class FixedBrush : public Brush {
		public:
			FixedBrush(ushort width, ushort height, QList<QPoint> points) : Brush(FIXED), m_brushMatrix(BrushMatrix(width, height, points)) {}
			void applyBrush(int i_, int j_, uint* colArray, QSize imageSize, uint col) const override;

			const BrushMatrix m_brushMatrix;
	};
}

#endif // BRUSH_H
