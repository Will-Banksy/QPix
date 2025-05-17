#include "LineTool.h"
#include <QImage>
#include "model/AppModel.h"
#include "utils/Utils.h"
#include "utils/PaintUtils.h"

LineTool::LineTool() : AbstractTool(), m_StartPoint(), m_EndPoint() {
	m_Name = "Line";
	m_Description = "Draws a line";
	m_IconPath = ":/assets/tools/line.svg";
	m_UsageType = ToolUsageType::Drag;

	m_Settings = new ToolSettings({ // TODO: It would be desirable to maintain the specified order of tool settings
		{ TS_LINE_UNIFORM, ToolSettingInfo {
			.Title = "Uniform Line",
			.Description = "Constrains lines to always use the same number of pixels per segment",
			.Value = TSVariant::newBool(false)
		}},
		{ TS_LINE_MAX_UNIFORM_SEGMENT_SIZE, ToolSettingInfo {
			.Title = "Max Segment Size",
			.Description = "When drawing uniform lines, set this to limit the segment size of lines, or 0 for unlimited.\nDoes not affect non-uniform lines.",
			.Value = TSVariant::newInRangeU32(TSInRange<u32> { .Start = 0, .End = 4096, .Value = 0 })
		}}
	});
}

LineTool::~LineTool() {
}

void LineTool::onDrag(const QImage& surface, QImage& buffer, QPoint pt, Qt::MouseButton button, ToolDragState state, AppModel* model) {
	QRgb colour = button == Qt::MouseButton::RightButton ? model->secondaryColour().rgba() : model->primaryColour().rgba();
	bool uniform = m_Settings->get(TS_LINE_UNIFORM).some().Value.toBool();
	u32 maxSegmentSize = m_Settings->get(TS_LINE_MAX_UNIFORM_SEGMENT_SIZE).some().Value.toInRangeU32().Value;

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
				PaintUtils::copyUniformLine(surface, buffer, start.x(), start.y(), oldEnd.x(), oldEnd.y(), maxSegmentSize);

				// Draw line from start point to new end point
				PaintUtils::drawUniformLine(buffer, start.x(), start.y(), pt.x(), pt.y(), colour, maxSegmentSize);
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
