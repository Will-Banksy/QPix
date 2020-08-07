#ifndef TOOL_H
#define TOOL_H

#include <string>
#include <QPoint>
#include <QMouseEvent>
#include <QColor>
#include "ui/Canvas.h"

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

		class ToolSettings {
			public:
				bool circleBrush;
				int tolerance;
				bool oneToOneRatio;
				bool fill;
				bool pixelPerfect;
				bool selectionAppend;
				bool interdisperse;
		};

		Tool(int id, Canvas* canvas);
		virtual ~Tool();
		std::string name;
		std::string description;
		std::string keyShortcut;
		std::string iconPath;
		int id;
		bool sizeMatters = false;
		ToolSettings settings;
		int triggerType;
		Canvas* canvas;

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
