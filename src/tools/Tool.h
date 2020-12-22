#ifndef TOOL_H
#define TOOL_H

#include <string>
#include <QPoint>
#include <QMouseEvent>
#include <QColor>
#include "ui/Canvas.h"
#include "ui/widgets/ToolButton.h"
#include "ToolOptionWidget.h"

// Define a macro to replace all CHECK_MOUSE_BUTTONS with the code after it at compile time
#define CHECK_MOUSE_BUTTONS if(currMouseButton != RIGHT && currMouseButton != LEFT) return;

namespace utils {} // I guess this is like a forward declaration of a namespace?
using namespace utils;

// TODO: Maybe add tool namespace?
/**
 * @todo write docs
 */
class Tool {
	public:
		enum MouseButton {
			LEFT,
			RIGHT,
			MIDDLE,
			NONE
		};

		enum ToolID {
			PENCIL,
			ERASER,
			FILL
		};

		class ToolSettings {
			public:
				bool circleBrush;
				int tolerance;
				bool oneToOneRatio; // A uniform line, if using the Line tool
				bool fill8Way;
				bool pixelPerfect;
				bool selectionAppend;
				bool interdisperse;
		};

		Tool(int id);
		virtual ~Tool();
		QString name;
		QString description;
		QString keyShortcut;
		QString iconPath;
		int id;
		bool sizeMatters = false;
		ToolSettings settings;
		int triggerType;
		Canvas* canvas;
// 		ToolButton* button;

		QList<ToolOptionWidget*> options;

	protected:
		QPoint curr;
		QPoint prev;
		QPoint start;
		MouseButton currMouseButton;

	private:
		bool firstDragEvent;

	public:
		// cPos is the transformed position of the mouse relative to the canvas grid - it's a grid coordinate
		virtual void onMousePressed(QMouseEvent* evt, QPoint& cPos);
		virtual void onMouseDragged(QMouseEvent* evt, QPoint& cPos);
		virtual void onMouseReleased(QMouseEvent* evt, QPoint& cPos);
		virtual void onMouseClicked(QMouseEvent* evt, QPoint& cPos);
		virtual void onDragCancel(QMouseEvent* evt, QPoint& cPos); // This method is called when you're dragging with one mouse button and press another
		virtual void onMouseMoved(QMouseEvent* evt, QPoint& cPos); // This method is called when you're moving the mouse when the mouse is not down
		bool isSelector();

	protected:
		MouseButton getMouseButton(QMouseEvent* evt);
		uint getColour();
};

#endif // TOOL_H
