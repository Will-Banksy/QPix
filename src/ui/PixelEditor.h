#ifndef PIXELEDITOR_H
#define PIXELEDITOR_H

#include <QMainWindow>

// TODO: Use tr() function for QStrings

class ToolPane;

class PixelEditor : public QMainWindow {
    Q_OBJECT

	public:
		explicit PixelEditor(QWidget *parent = nullptr);
		~PixelEditor() override;

	private:
		// Dock Widgets
		ToolPane* toolPane;

		// The menu bar
		QMenuBar* menuBar;

		// Actions
		QAction* quitAct;
		QAction* showToolsAct;

		void createActions();
		void setupMenus();
		void addToolBars();

	public slots:
		void quit();
		void showTools();
};

#endif // PIXELEDITOR_H
