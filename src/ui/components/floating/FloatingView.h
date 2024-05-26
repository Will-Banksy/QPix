#ifndef FLOATINGVIEW_H
#define FLOATINGVIEW_H

#include <QWidget>

// enum FloatingWidgetType {
// 	Info,
// 	ColourDialog
// };

enum class FloatingPosition {
	Right,
	Left,
	Bottom,
	Top,
	Unspecified
};

class AppModel;
class FloatingInfoView;

class FloatingView : public QWidget {
	Q_OBJECT

public:
	explicit FloatingView(QWidget* parent, AppModel* model);
	~FloatingView();

public slots:
	/// Shows a floating window near the src widget, displaying the title in bold above the body text
	void showFloatingInfo(QWidget* src, const QString& title, const QString& body, FloatingPosition position);

	/// Shows a floating window near the src widget, displaying a colour selector set to the current colour
	void showColourSelect(QWidget* src, QColor current);

	/// Hides the floating window
	void hide();

private:
	FloatingInfoView* m_InfoView;

	/// Reposition this floating view to near the src widget
	void reposition(QWidget* src, FloatingPosition position);
};

#endif // FLOATINGVIEW_H