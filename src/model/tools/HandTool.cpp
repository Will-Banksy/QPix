#include "HandTool.h"
#include "model/AppModel.h"

HandTool::HandTool() {
	m_Name = "Scroll Hand";
	m_Description = "Allows you to drag the canvas to scroll it";
	m_IconPath = ":/data/tools/move.svg";
	m_UsageType = ToolUsageType::None;

	m_Settings = new ToolSettings({
	});
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
