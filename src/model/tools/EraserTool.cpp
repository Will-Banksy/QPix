#include "EraserTool.h"
#include "utils/PaintUtils.h"
#include <QColor>
#include <QImage>

EraserTool::EraserTool() {
	m_Name = "Eraser";
	m_Description = "Erases pixels";
	m_IconPath = ":/data/tools/eraser.svg";
	m_UsageType = ToolUsageType::Drag;

	m_Settings = new ToolSettings({
		{ TS_ERASER_OPACITY, ToolSettingInfo {
			.Title = "Opacity",
			.Description = "The amount (as a percentage of 255) of opacity that the eraser removes from pixels",
			.Value = TSVariant::newInRangeU32(TSInRange<u32> { .Start = 0, .End = 255, .Value = 255 })
		}}
	});
}

EraserTool::~EraserTool() {
}

void EraserTool::onDrag(const QImage& surface, QImage& buffer, QPoint pt, Qt::MouseButton button, ToolDragState state, AppModel* model) {
	QPoint start = pt;

	switch(state) {
		case ToolDragState::Press: {
			PaintUtils::reset(surface.size());
			m_PrevPt = Option<QPoint>::newSome(pt);
			break;
		}
		case ToolDragState::Drag: {
			start = m_PrevPt.some();
			m_PrevPt = Option<QPoint>::newSome(pt);
			break;
		}
		case ToolDragState::Release: {
			start = m_PrevPt.some();
			m_PrevPt = Option<QPoint>::newNone();
			break;
		}
		case ToolDragState::Cancel: {
			return;
		}
	}

	u32 opacity = m_Settings->get(TS_ERASER_OPACITY).some().Value.toInRangeU32().Value;

	PaintUtils::affectLine(buffer, start.x(), start.y(), pt.x(), pt.y(), [this, opacity](QRgb col) {
		f32 opacityF = f32(255 - opacity) / 255.0;

		u8 alpha = qAlpha(col);
		alpha = u8(f32(alpha) * opacityF);
		col = qRgba(qRed(col), qGreen(col), qBlue(col), alpha);

		return col;
	}, true);
}
