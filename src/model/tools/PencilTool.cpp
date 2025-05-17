#include "PencilTool.h"
#include <QImage>
#include "model/AppModel.h"
#include "utils/Utils.h"
#include "utils/PaintUtils.h"

PencilTool::PencilTool() : AbstractTool(), m_CurrentStroke(QList<QPoint>()) {
	m_Name = "Pencil";
	m_Description = "Draws pixels";
	m_IconPath = ":/assets/tools/pencil.svg";
	m_UsageType = ToolUsageType::Drag;

	m_Settings = new ToolSettings({
		{ TS_PENCIL_PIXELPERFECT, ToolSettingInfo {
			.Title = "Pixel Perfect",
			.Description = "Removes jaggies (right angles of pixels) from your strokes as you make them",
			.Value = TSVariant::newBool(false),
		}}
	});
}

PencilTool::~PencilTool() {
}

void PencilTool::onDrag(const QImage& surface, QImage& buffer, QPoint pt, Qt::MouseButton button, ToolDragState state, AppModel* model) {
	QRgb colour = button == Qt::MouseButton::RightButton ? model->secondaryColour().rgba() : model->primaryColour().rgba();
	bool pixelPerfect = m_Settings->get(TS_PENCIL_PIXELPERFECT).some().Value.toBool();

	// NOTE: Will be buggy in case of the pixelPerfect setting switching mid-stroke
	//       Perhaps introduce a "locking" mechanism into ToolSettings, where certain settings can be "locked" which disables their hotkeys
	//       and respective widgets (emits settingLocked signal) - This will mean that we do not have to consider mid-drag setting changes
	//       and making those work nicely, we can just disable them

	switch(state) {
		case ToolDragState::Press: {
			PaintUtils::reset(surface.size());
			PaintUtils::drawLine(buffer, pt.x(), pt.y(), pt.x(), pt.y(), colour, pixelPerfect ? &m_CurrentStroke : nullptr, true);

			if(!pixelPerfect) {
				m_CurrentStroke.push_back(pt);
			}
			break;
		}
		case ToolDragState::Release:
		case ToolDragState::Drag: {
			// Draw line from last point to new point
			QPoint prev = m_CurrentStroke.last();
			PaintUtils::drawLine(buffer, prev.x(), prev.y(), pt.x(), pt.y(), colour, pixelPerfect ? &m_CurrentStroke : nullptr, false);

			if(!pixelPerfect) {
				m_CurrentStroke.push_back(pt);
			}

			// If pixel perfect corrections are selected to be applied, then do so
			if(pixelPerfect) {
				PaintUtils::pixelPerfectCorrect(surface, buffer, m_CurrentStroke);
			}

			if(state == ToolDragState::Release) {
				m_CurrentStroke.clear();
			}
			break;
		}
		case ToolDragState::Cancel: {
			m_CurrentStroke.clear();
			break;
		}
	}
}
