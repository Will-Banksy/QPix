#ifndef LINETOOL_H
#define LINETOOL_H

#include "AbstractTool.h"

const static QString TS_LINE_UNIFORM = QString("Uniform Line");

class LineTool : public AbstractTool {
	Q_OBJECT

public:
	explicit LineTool();
	~LineTool() override;

    void onDrag(const QImage& surface, QImage& buffer, QPoint pt, Qt::MouseButton button, ToolDragState state, AppModel* model) override;

private:
	/// Line start point (for current line being drawn)
	QPoint m_StartPoint;
	/// Line end point (for current line being drawn)
	QPoint m_EndPoint;
};

#endif // LINETOOL_H