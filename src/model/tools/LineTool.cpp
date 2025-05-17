#include "LineTool.h"
#include <QImage>
#include "model/AppModel.h"
#include "utils/Utils.h"
#include "utils/PaintUtils.h"

LineTool::LineTool() : AbstractTool(), m_StartPoint(), m_EndPoint() {
	m_Name = "Line";
	m_Description = "Draws a line";
	m_IconPath = ":/data/tools/line.svg";
	m_UsageType = ToolUsageType::Drag;

	m_Settings = new ToolSettings({
		{ TS_LINE_UNIFORM, TSVariant::newBool(false) }
	});
}

LineTool::~LineTool() {
}

void LineTool::onDrag(const QImage& surface, QImage& buffer, QPoint pt, Qt::MouseButton button, ToolDragState state, AppModel* model) {
	QRgb colour = button == Qt::MouseButton::RightButton ? model->secondaryColour().rgba() : model->primaryColour().rgba();
	bool uniform = m_Settings->get(TS_LINE_UNIFORM).some().toBool();

	switch(state) {
		case ToolDragState::Press: {
			PaintUtils::reset(surface.size());
			PaintUtils::drawLine(buffer, pt.x(), pt.y(), pt.x(), pt.y(), colour, nullptr);

			m_StartPoint = pt;
			m_EndPoint = pt;

			break;
		}
		case ToolDragState::Release:
		case ToolDragState::Drag: {
			const QPoint& start = m_StartPoint;
			const QPoint& oldEnd = m_EndPoint;

			if(uniform) {
				// Erase old line
				PaintUtils::copyUniformLine(surface, buffer, start.x(), start.y(), oldEnd.x(), oldEnd.y());

				// Draw line from start point to new end point
				PaintUtils::drawUniformLine(buffer, start.x(), start.y(), pt.x(), pt.y(), colour);
			} else {
				// Erase old line
				PaintUtils::copyLine(surface, buffer, start.x(), start.y(), oldEnd.x(), oldEnd.y());

				// Draw line from start point to new end point
				PaintUtils::drawLine(buffer, start.x(), start.y(), pt.x(), pt.y(), colour);
			}

			m_EndPoint = pt;

			break;
		}
		case ToolDragState::Cancel: {
			break;
		}
	}
}
