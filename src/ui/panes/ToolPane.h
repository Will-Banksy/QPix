#ifndef TOOLPANE_H
#define TOOLPANE_H

#include <QWidget>
#include <QDockWidget>
#include "ui/FlowLayout.h"

/**
 * @todo write docs
 */
class ToolPane : public QDockWidget {
	public:
		~ToolPane() = default;
		ToolPane(QWidget* parent = nullptr);

// 	protected:
// 		void paintEvent(QPaintEvent* event) override;

	private:
		void addToolButtons(FlowLayout* layout);
};

#endif // TOOLPANE_H
