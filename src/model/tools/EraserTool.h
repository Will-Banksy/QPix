#ifndef ERASERTOOL_H
#define ERASERTOOL_H

#include "AbstractTool.h"
#include "utils/Option.h"

const static QString TS_ERASER_OPACITY = QString("eraser.opacity");

class EraserTool : public AbstractTool {
	Q_OBJECT

public:
	explicit EraserTool();
	~EraserTool() override;

    void onDrag(const QImage& surface, QImage& buffer, QPoint pt, Qt::MouseButton button, ToolDragState state, AppModel* model) override;

private:
	Option<QPoint> m_PrevPt;
};

#endif // ERASERTOOL_H