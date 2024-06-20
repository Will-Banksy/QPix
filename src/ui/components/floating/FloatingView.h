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

typedef std::function<void(const QColor&)> ColourChangeCallback;

class AppModel;
class FloatingTooltipView;

class FloatingView : public QWidget { // TODO: Sort out terminology and make it consistent
	Q_OBJECT

public:
	explicit FloatingView(QWidget* parent, AppModel* model);
	~FloatingView();

public slots:
	/// Shows a tooltip near the src widget, displaying the title in bold above the body text
	void showFloatingInfo(QWidget* src, const QString& title, const QString& body, FloatingPosition position);

	/// Hides the tooltip
	void hideTooltips();

	/// Shows a popup
	void showColourSelectPopup(QWidget* src, const QColor& colour, ColourChangeCallback callback);

	/// Dismisses the popup
	void dismissPopup();

protected:
	void mousePressEvent(QMouseEvent* event) override;

private:
	FloatingTooltipView* m_TooltipView;
};

#endif // FLOATINGVIEW_H