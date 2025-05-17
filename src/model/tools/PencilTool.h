#ifndef PENCILTOOL_H
#define PENCILTOOL_H

#include "AbstractTool.h"

const static QString TS_PENCIL_PIXELPERFECT = QString("pencil.pixel-perfect");

class PencilTool : public AbstractTool {
	Q_OBJECT

public:
	explicit PencilTool();
	~PencilTool() override;

    void onDrag(const QImage& surface, QImage& buffer, QPoint pt, Qt::MouseButton button, ToolDragState state, AppModel* model) override;

private:
	/// List of points comprising each line in the current stroke, for pixel perfect strokes
	QList<QPoint> m_CurrentStroke;
};

#endif // PENCILTOOL_H