#ifndef UI_H
#define UI_H

#include <QObject>
#include <QList>
#include <QHash>
#include <QVariant>

class QAction;
class Window;
class CanvasView;
class ToolPane;
class QStackedWidget;
class ToolButton;
class TabbedProjectView;
class QDockWidget;
class Tool;
class QShortcut;
class ToolOptionWidget;
class Project;

class UI : public QObject {
	Q_OBJECT

public:
	UI(Window* window);
	~UI();

public:
	Window* window; // Does not own this pointer
	CanvasView* canvasView; // Owns this
	QDockWidget* toolDock;
	QStackedWidget* toolConfigStack;
	QList<ToolButton*> toolButtons;
	TabbedProjectView* tabbedView;
	QList<QList<ToolOptionWidget*>> toolOptionWidgets; // Organised by toolId, then by option widget id

	QHash<QString, QAction*> actions;
	QHash<Tool*, QShortcut*> toolShortcuts;

	// TODO Some data structure that can hold the contents of what I want in the status bar
	// Status bar will hold icons and strings associated with the icons, such as a coordinates icon and the coordinates as a string
	// Can just hold strings too I guess (at least for some entries)

	// Setup functions (could be private?)
	void setupUI();
	void createMenus();
	void createActions();
	void createDocks();
	void createToolbars();

	// Other functions
	void switchToolUI(int selectedTool);
	void changeToolConfigUI(int tool, int configIndex, QVariant value);
	bool windowCanClose();
	void switchProject(Project* selectedProject);

public slots:
	void showToolsDock();
	void newWindow();
	void newTab();
	void closeWindow();
	void closeTab();
};

#endif // UI_H
