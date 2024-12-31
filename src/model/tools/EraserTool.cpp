#include "EraserTool.h"
#include "utils/PaintUtils.h"
#include <QColor>

EraserTool::EraserTool() {
	m_Name = "Eraser";
	m_Description = "Erases pixels";
	m_IconPath = ":/data/tools/eraser.svg";
	m_UsageType = ToolUsageType::Drag;

	m_Settings = new ToolSettings({
	});
}

EraserTool::~EraserTool() {
}

void EraserTool::onDrag(const QImage& surface, QImage& buffer, QPoint pt, Qt::MouseButton button, ToolDragState state, AppModel* model) {
	QPoint start = pt;

	switch(state) {
		case ToolDragState::Press: {
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

	PaintUtils::drawLine(buffer, start.x(), start.y(), pt.x(), pt.y(), QColorConstants::Transparent.rgba());
}
