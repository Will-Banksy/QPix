#include "AbstractTool.h"
#include "PencilTool.h"
#include "BucketTool.h"
#include "HandTool.h"
#include "PipetteTool.h"

AbstractTool::AbstractTool() : QObject() {
}

AbstractTool::~AbstractTool() {
}

void AbstractTool::onDrag(const QImage& surface, QImage& buffer, QPoint pt, Qt::MouseButton button, ToolDragState state, AppModel* model) {
}

void AbstractTool::onClick(const QImage& surface, QImage& buffer, QPoint pt, Qt::MouseButton button, AppModel* model) {
}

void AbstractTool::onSelectedChanged(bool selected, AppModel* model) {
}

QList<AbstractTool*> AbstractTool::initialiseTools() {
	return {
        (AbstractTool*)new PencilTool(),
		(AbstractTool*)new PipetteTool(),
		(AbstractTool*)new BucketTool(),
		(AbstractTool*)new HandTool(),
	};
}

const QString& AbstractTool::name() const {
	return m_Name;
}

const QString& AbstractTool::description() const {
	return m_Description;
}

const QString& AbstractTool::iconPath() const {
	return m_IconPath;
}

const ToolUsageType& AbstractTool::usageType() const {
	return m_UsageType;
}

ToolSettings* AbstractTool::settings() const {
	return m_Settings;
}
