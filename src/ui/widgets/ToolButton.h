#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QPushButton>

/**
 * @todo write docs
 */
class ToolButton : public QPushButton {
// 	protected:
		Q_OBJECT

	public:
		explicit ToolButton(QWidget* parent = nullptr);
		~ToolButton() override;

	public slots:
		void onToggled(bool checked);
};

#endif // TOOLBUTTON_H
