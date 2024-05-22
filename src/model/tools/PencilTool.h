#include "Tool.h"

class PencilTool : public Tool {
	Q_OBJECT

public:
	explicit PencilTool();
	~PencilTool() override;

	void onDrag(QImage& surface, QPoint pt, ToolDragState state, AppModel* model) override;
};