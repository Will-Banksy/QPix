#ifndef TOOLPANE_H
#define TOOLPANE_H

#include <QWidget>
#include <QDockWidget>
#include "ui/FlowLayout2.h"

/**
 * @todo write docs
 */
class ToolPane : public QDockWidget {
public:
	ToolPane(QWidget* parent = nullptr);
	virtual ~ToolPane();
};

#endif // TOOLPANE_H
