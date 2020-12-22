#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QToolButton>
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

	public slots:
		void onToggled(bool checked);
};

#endif // TOOLBUTTON_H
