#ifndef TOOLPANE_H
#define TOOLPANE_H

#include <QWidget>
#include <QDockWidget>

/**
 * @todo write docs
 */
class ToolPane : public QDockWidget {
	public:
		~ToolPane() = default;
		ToolPane(QWidget* parent = nullptr);
};

#endif // TOOLPANE_H
