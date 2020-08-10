#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QToolButton>
// #include "tools/Tool.h"

class Tool;

/**
 * @todo write docs
 */
class ToolButton : public QToolButton {
	Q_OBJECT

	public:
		explicit ToolButton(Tool& tool, QWidget* parent = nullptr);
		~ToolButton() override;
		Tool& tool;

	public slots:
		void onToggled(bool checked);
};

#endif // TOOLBUTTON_H
