#include "PencilTool.h"
#include <QImage>
#include "model/AppModel.h"
#include "utils/Utils.h"

PencilTool::PencilTool() : AbstractTool(), m_CurrentStroke(QList<QPoint>()) {
	m_Name = "Pencil";
	m_Description = "Draws pixels";
	m_IconPath = ":/tools/pencil.png";
	m_UsageType = ToolUsageType::Drag;
}

PencilTool::~PencilTool() {
}

void PencilTool::onDrag(QImage& surface, QPoint pt, Qt::MouseButton button, ToolDragState state, AppModel* model) {
	QRgb colour = button == Qt::MouseButton::RightButton ? model->secondaryColour().rgba() : model->primaryColour().rgba();

	switch(state) {
		case ToolDragState::Press: {
			if(utils::contains(surface, pt)) {
				surface.setPixel(pt, colour);
			}
			m_CurrentStroke.push_back(pt);
			break;
		}
		case ToolDragState::Release:
		case ToolDragState::Drag: {
			QPoint prev = m_CurrentStroke.last();
			QList<QPoint> line = utils::plotLine(prev.x(), prev.y(), pt.x(), pt.y());
			line.removeFirst(); // Remove the start point that's the same as the end of the previous line

			QRgb* bytes = (QRgb*)surface.scanLine(0); // Assumes a 32-bit format

			for(QPoint& linePt : line) {
				if(utils::contains(surface, linePt)) {
					bytes[linePt.x() + linePt.y() * surface.width()] = colour;
				}
			}
			if(state == ToolDragState::Drag) {
				m_CurrentStroke.append(line);
			} else {
				m_CurrentStroke.clear();
			}
			break;
		}
		case ToolDragState::Cancel: {
			m_CurrentStroke.clear();
		}
	}
}
