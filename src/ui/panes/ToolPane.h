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
		~ToolPane() = default;
		ToolPane(QWidget* parent = nullptr);

	private:
		void addToolButtons(FlowLayout2* layout);
};

#endif // TOOLPANE_H
