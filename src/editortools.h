#ifndef EDITORTOOLS_H
#define EDITORTOOLS_H

#include <QColor>
#include <src/tools/tool.h>

// class Canvas;
// class Tool; // Forward declaration - can only be used when defining references or pointers to the class

/**
 * @todo write docs
 */
class EditorTools {
	private:
		EditorTools();
		~EditorTools();

	public:
		static uint primaryColour;
		static uint secondaryColour;
		static Tool* selectedTool;
		static QList<Tool*> tools;
		static void initTools(Canvas* canvas);
};

#endif // EDITORTOOLS_H
