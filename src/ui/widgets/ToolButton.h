#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QToolButton>
// #include "tools/Tool.h"

#include <QWidget>

class Tool;

/**
 * @todo write docs
 */
class ToolButton : public QToolButton { // Hang on... ToolButtons are basically glorified radio buttons. I've got the code working now though, so no need to change them
	Q_OBJECT

	public:
		explicit ToolButton(Tool* tool, QWidget* parent = nullptr);
		~ToolButton() override;
		Tool* tool;
		void setCheckedSafe(bool checked, bool blockSignal = false);

	private:
		bool checkedSafely;
		bool blockNextSignal;

	public slots:
		void onToggled(bool checked);
};

class Thing : public QWidget {
public:
	Thing() {

	}
};

#endif // TOOLBUTTON_H
