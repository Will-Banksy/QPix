#ifndef BRUSH_H
#define BRUSH_H

#include "BrushMatrix.h"

/**
 * @todo write docs
 */
namespace utils {
	enum BrushType {
		/// The default brush is a square of various sizes
		DEFAULT,
		/// A custom BrushMatrix is specified for this type
		CUSTOM,
		/// Like the default but only 1px size
		POINT,
		/// No brush
		NONE
	};

	class DefaultBrush {
		public:
			DefaultBrush() = default;
			const BrushType type = DEFAULT;
	};

	class CustomBrush : public DefaultBrush {
		public:
			CustomBrush(ushort width, ushort height, QList<QPoint> points);

			const BrushType type = CUSTOM;
			const BrushMatrix brushMatrix;
	};

	class Brush1Px : public DefaultBrush {
		public:
			Brush1Px() : DefaultBrush() {}

			const BrushType type = POINT;
	};
}

#endif // BRUSH_H
