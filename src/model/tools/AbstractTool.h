#ifndef ABSTRACTTOOL_H
#define ABSTRACTTOOL_H

#include <QObject>
#include <QPoint>
#include "ToolSettings.h"

class AppModel;

enum class ToolUsageType {
	None,
	Drag,
	Click,
};

enum class ToolDragState {
	Press,
	Drag,
	Release,
	Cancel
};

class AbstractTool : public QObject {
	Q_OBJECT

public:
    explicit AbstractTool();
    virtual ~AbstractTool();

	// TODO: Do I allow tools to define their own cancellation behaviour? Could allow some nice optimisation

	/// Called for tools that have usage type Drag, when the user is dragging the mouse
	/// while a mouse button is pressed. Also called on initial button press, on release,
	/// and on drag cancel - which of these is the case is indicated by the "state" parameter.
	///
	/// Drag cancel occurs when a drag was started with one button and another button was pressed
	/// without releasing the first button. The image state will be automatically reset in most
	/// situations, meaning that tools should not edit the image when drag cancel occurs
	virtual void onDrag(QImage& surface, QPoint pt, Qt::MouseButton button, ToolDragState state, AppModel* model);

	/// Called for tools that have usage type Click, when the user clicks on a pixel
	virtual void onClick(QImage& surface, QPoint pt, Qt::MouseButton button, AppModel* model);

	/// Called when a tool is selected (true) or deselected (false). Most tools will not use this
	virtual void onSelectedChanged(bool selected, AppModel* model);

    static QList<AbstractTool*> initialiseTools();

	const QString& name() const;
	const QString& description() const;
	const QString& iconPath() const;
	const ToolUsageType& usageType() const;
	ToolSettings* settings() const;

protected:
	QString m_Name = "Unnamed Tool";
	QString m_Description = "This tool has not been given a description";
	QString m_IconPath = ":/data/tools/pencil.png";
	ToolUsageType m_UsageType = ToolUsageType::Click;
	ToolSettings* m_Settings = nullptr;

};

#endif // ABSTRACTTOOL_H