#include "ToolPane.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include "tools/Tool.h"
#include "EditorTools.h"
#include "ui/widgets/ToolButton.h"
#include <QSizeGrip>
#include <QPainter>

ToolPane::ToolPane(QWidget* parent) : QDockWidget(parent) {
	setWindowTitle("Tools");

	// TODO: Somehow make it so that QDockWidgets are easily resizeable. Currently, you can use the Dock Widget titlebar
	// QSizeGrips might be the way to go
// 	QSizeGrip resizeHandle(this);
// 	resizeHandle.setVisible(true);

// 	widget->setProperty("class", "DockContent"); // For if I use stylesheets
}

ToolPane::~ToolPane() {
}
