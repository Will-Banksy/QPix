#ifndef EDITORTOOLS_H
#define EDITORTOOLS_H

#include <QColor>
#include "tools/Tool.h"
#include "utils/Brush.h"

// class Canvas;
// class Tool; // Forward declaration - can only be used when defining references or pointers to the class

/**
 * @todo write docs
 */
class EditorTools {
	private:
		EditorTools() = delete;
		~EditorTools() = delete;

	public:
		static uint primaryColour;
		static uint secondaryColour;
		static Tool* selectedTool;
		static QList<Tool*> tools;
		static Brush brush;
		static ushort brushWidth;
		static ushort brushHeight;

		static void initTools(Canvas* canvas);
		static void setBrushSize(ushort size);
		static void setBrushSize(ushort width, ushort height);
};

#endif // EDITORTOOLS_H