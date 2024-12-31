#include "PipetteTool.h"
#include <QImage>
#include "model/AppModel.h"
#include "ToolSettings.h"

PipetteTool::PipetteTool() : AbstractTool() {
	m_Name = "Pipette";
	m_Description = "Picks a colour from the canvas";
	m_IconPath = ":/data/tools/svg/pipette.svg"; // TODO: Needs new icon
	m_UsageType = ToolUsageType::Click;
	m_Settings = new ToolSettings({
		{ TS_PIPETTE_COPYALPHA, QVariant(true) }
	});
}

PipetteTool::~PipetteTool() {
}

void PipetteTool::onClick(const QImage& surface, QImage& buffer, QPoint pt, Qt::MouseButton button, AppModel* model) {
	QColor col = surface.pixelColor(pt);
	bool copyAlpha = m_Settings->get(TS_PIPETTE_COPYALPHA).unwrap()->toBool();

	if(button == Qt::MouseButton::RightButton) {
		if(!copyAlpha) {
			int alpha = model->secondaryColour().alpha();
			col.setAlpha(alpha);
		}
		model->setSecondaryColour(col);
	} else {
		if(!copyAlpha) {
			int alpha = model->primaryColour().alpha();
			col.setAlpha(alpha);
		}
		model->setPrimaryColour(col);
	}
}

