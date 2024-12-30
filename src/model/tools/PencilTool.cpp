#include "PencilTool.h"
#include <QImage>
#include "model/AppModel.h"
#include "utils/Utils.h"
#include "utils/PaintUtils.h"

PencilTool::PencilTool() : AbstractTool(), m_CurrentStroke(QList<QPoint>()) {
	m_Name = "Pencil";
	m_Description = "Draws pixels";
	m_IconPath = ":/data/tools/pencil.png";
	m_UsageType = ToolUsageType::Drag;
	m_Settings = new ToolSettings({
		{ TS_PENCIL_PIXELPERFECT, QVariant(false) }
	});
}

PencilTool::~PencilTool() {
}

void PencilTool::onDrag(const QImage& surface, QImage& buffer, QPoint pt, Qt::MouseButton button, ToolDragState state, AppModel* model) {
	QRgb colour = button == Qt::MouseButton::RightButton ? model->secondaryColour().rgba() : model->primaryColour().rgba();

	switch(state) {
		case ToolDragState::Press: {
			PaintUtils::reset(surface.size());
			PaintUtils::drawLine(buffer, pt.x(), pt.y(), pt.x(), pt.y(), colour, &m_CurrentStroke, true);
			m_CurrentStroke.push_back(pt);
			break;
		}
		case ToolDragState::Release:
		case ToolDragState::Drag: {
			// Draw line from last point to new point
			QPoint prev = m_CurrentStroke.last();
			PaintUtils::drawLine(buffer, prev.x(), prev.y(), pt.x(), pt.y(), colour, &m_CurrentStroke, false);

			// If pixel perfect corrections are selected to be applied, then do so
			if(m_Settings->get(TS_PENCIL_PIXELPERFECT).unwrap()->toBool()) {
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
