#include "AbstractTool.h"
#include "PencilTool.h"
#include "BucketTool.h"
#include "HandTool.h"

AbstractTool::AbstractTool() : QObject() {
}

AbstractTool::~AbstractTool() {
}

void AbstractTool::onDrag(QImage& surface, QPoint pt, Qt::MouseButton button, ToolDragState state, AppModel* model) {
}

void AbstractTool::onClick(QImage& surface, QPoint pt, Qt::MouseButton button, AppModel* model) {
}

void AbstractTool::onSelectedChanged(bool selected, AppModel* model) {
}

QList<AbstractTool*> AbstractTool::initialiseTools() {
	return {
        (AbstractTool*)new PencilTool(),
		(AbstractTool*)new BucketTool(),
		(AbstractTool*)new HandTool(),
	};
}

QString AbstractTool::name() {
	return m_Name;
}

QString AbstractTool::description() {
	return m_Description;
}

QString AbstractTool::iconPath() {
	return m_IconPath;
}

ToolUsageType AbstractTool::usageType() {
	return m_UsageType;
}
