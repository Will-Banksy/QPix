#ifndef FLOATINGVIEW_H
#define FLOATINGVIEW_H

#include <QWidget>

enum class FloatingPosition {
	Right,
	Left,
	Bottom,
	Top,
	ScreenCentre,
	Unspecified
};

class AppModel;
class FloatingTooltipView;

class FloatingView : public QWidget {
	Q_OBJECT

public:
	explicit FloatingView(QWidget* parent, AppModel* model);
	~FloatingView();

public slots:
	/// Shows a floating window near the src widget, displaying the title in bold above the body text
	void showFloatingInfo(QWidget* src, const QString& title, const QString& body, FloatingPosition position);

	/// Hides the floating windows
	void hideTooltips();

private:
	FloatingTooltipView* m_TooltipView;
};

#endif // FLOATINGVIEW_H