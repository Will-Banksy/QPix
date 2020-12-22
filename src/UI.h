#ifndef UI_H
#define UI_H

#include <QAction>

class Window;
class CanvasView;
class ToolPane;
class QStackedWidget;
class ToolButton;

class UI : public QObject {
	Q_OBJECT

public:
	UI(Window* window);
	~UI();

public:
	Window* window; // Does not own this pointer
	CanvasView* canvasPane;
	ToolPane* toolDock; // TODO Rename class ToolPane to ToolDock
	QStackedWidget* toolConfigStack;
	QList<ToolButton*> toolButtons;

	QHash<QString, QAction*> actions;

	// Setup functions (could be private?)
	void setupUI();
	void createMenus();
	void createActions();
	void createDocks();
	void createToolbars();

	// Other functions
	void switchToolUI(int selectedTool);

public slots:
	void showToolsDock();
	void newWindow();
};

#endif // UI_H
