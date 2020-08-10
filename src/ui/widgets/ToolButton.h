#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QPushButton>
#include "tools/Tool.h"

/**
 * @todo write docs
 */
class ToolButton : public QPushButton {
	Q_OBJECT

	public:
		explicit ToolButton(Tool* tool, QWidget* parent = nullptr);
		~ToolButton() override;
		Tool* tool;

	public slots:
		void onToggled();
};

#endif // TOOLBUTTON_H
