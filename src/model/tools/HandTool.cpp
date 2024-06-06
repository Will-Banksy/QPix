#include "HandTool.h"
#include "model/AppModel.h"

HandTool::HandTool() {
	m_Name = "Scroll Hand";
	m_Description = "Allows you to drag the canvas to scroll it";
	m_IconPath = ":/data/tools/move.png";
	m_UsageType = ToolUsageType::None;
}

HandTool::~HandTool() {
}

void HandTool::onSelectedChanged(bool selected, AppModel* model) {
	if(selected) {
		emit model->canvasDragModeRequested(true);
	} else {
		emit model->canvasDragModeRequested(false);
	}
}
