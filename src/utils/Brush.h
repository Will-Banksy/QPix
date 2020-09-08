#ifndef BRUSH_H
#define BRUSH_H

/**
 * @todo write docs
 */
namespace utils {
	enum BrushType {
		DEFAULT,
		CUSTOM
	};

	class Brush {
		public:
			Brush() = default;
			const BrushType type = DEFAULT;
	};

	class CustomBrush : public Brush {
		CustomBrush() : Brush() {};

		const BrushType type = CUSTOM;
		const bool** brushMatrix;
	};
}

#endif // BRUSH_H
