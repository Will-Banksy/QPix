#ifndef TOOL_H
#define TOOL_H

#include <QObject>
#include <QPoint>

class AppModel;

enum class ToolUsageType {
	Drag,
	Click,
};

enum class ToolDragState {
	Press,
	Drag,
	Release,
	Cancel
};

class Tool : public QObject {
	Q_OBJECT

public:
	explicit Tool();
	virtual ~Tool() = 0;

	/// Called for tools that have usage type Drag, when the user is dragging the mouse
	/// while a mouse button is pressed. Also called on initial button press, on release,
	/// and on drag cancel - which of these is the case is indicated by the "state" parameter.
	///
	/// Drag cancel occurs when a drag was started with one button and another button was pressed
	/// without releasing the first button. Tools are expected to cancel operation in this case
	virtual void onDrag(QImage& surface, QPoint pt, ToolDragState state, AppModel* model);

	/// Called for tools that have usage type Click, when the user clicks on a pixel
	virtual void onClick(QImage& surface, QPoint pt, AppModel* model);

	static QList<Tool*> initialiseTools();

	QString name();
	QString description();
	QString iconPath();
	ToolUsageType usageType();

protected:
	ToolUsageType m_UsageType = ToolUsageType::Click;
	QString m_Name = "Unnamed Tool";
	QString m_Description = "This tool has not been given a description";
	QString m_IconPath = ":/tools/pencil.png";
};

#endif // TOOL_H