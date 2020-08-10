#ifndef PIXELEDITOR_H
#define PIXELEDITOR_H

#include <QMainWindow>

class ToolPane;

class PixelEditor : public QMainWindow {
    Q_OBJECT

	public:
		explicit PixelEditor(QWidget *parent = nullptr);
		~PixelEditor() override;

	private:
		ToolPane* toolPane;

		void setupMenus();
};

#endif // PIXELEDITOR_H
