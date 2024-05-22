#include "Tool.h"

Tool::Tool() : QObject() {
}

void Tool::onDrag(QImage& surface, QPoint pt, ToolDragState state, AppModel* model) {
}

void Tool::onClick(QImage& surface, QPoint pt, AppModel* model) {
}

QList<Tool*> Tool::initialiseTools() {
	return {
		(Tool*)nullptr
	};
}

QString Tool::name() {
	return m_Name;
}

QString Tool::description() {
	return m_Description;
}

QString Tool::iconPath() {
	return m_IconPath;
}

ToolUsageType Tool::usageType() {
	return m_UsageType;
}