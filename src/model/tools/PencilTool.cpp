#include "PencilTool.h"
#include <QImage>
#include "model/AppModel.h"

PencilTool::PencilTool() : Tool() {
	m_Name = "Pencil";
	m_Description = "Draws pixels";
	m_IconPath = ":/tools/pencil.png";
	m_UsageType = ToolUsageType::Drag;
}

PencilTool::~PencilTool() {
}

void PencilTool::onDrag(QImage& surface, QPoint pt, ToolDragState state, AppModel* model) {
	surface.setPixel(pt, model->primaryColour().rgb());
}