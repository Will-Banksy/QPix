#ifndef UI_H
#define UI_H

#include <QObject>
#include <QList>
#include <QHash>

class QAction;
class Window;
class CanvasView;
class ToolPane;
class QStackedWidget;
class ToolButton;
class TabbedProjectView;
class QDockWidget;

class UI : public QObject {
	Q_OBJECT

public:
	UI(Window* window);
	~UI();

public:
	Window* window; // Does not own this pointer
	CanvasView* canvasView; // Owns this
	QDockWidget* toolDock; // TODO Rename class ToolPane to ToolDock
	QStackedWidget* toolConfigStack;
	QList<ToolButton*> toolButtons;
	TabbedProjectView* tabbedView;

	QHash<QString, QAction*> actions;

	// Setup functions (could be private?)
	void setupUI();
	void createMenus();
	void createActions();
	void createDocks();
	void createToolbars();

	// Other functions
	void switchToolUI(int selectedTool);
	bool windowCanClose();

public slots:
	void showToolsDock();
	void newWindow();
	void newTab();
	void closeWindow();
	void closeTab();
};

#endif // UI_H
