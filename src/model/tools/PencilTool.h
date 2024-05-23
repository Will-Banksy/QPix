#include "AbstractTool.h"

class PencilTool : public AbstractTool {
	Q_OBJECT

public:
	explicit PencilTool();
	~PencilTool() override;

    void onDrag(QImage& surface, QPoint pt, Qt::MouseButton button, ToolDragState state, AppModel* model) override;

private:
	/// List of points comprising each line in the current stroke, for pixel perfect strokes
	QList<QPoint> m_CurrentStroke;
};
