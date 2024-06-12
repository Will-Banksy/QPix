#ifndef FLOATINGTOOLTIPVIEW_H
#define FLOATINGTOOLTIPVIEW_H

#include "../FloatingView.h"

class AppModel;
class FloatingInfoView;

class FloatingTooltipView : public QWidget {
	Q_OBJECT

public:
	explicit FloatingTooltipView(QWidget* parent, AppModel* model);
	~FloatingTooltipView() override;

public slots:
	/// Shows a floating window near the src widget, displaying the title in bold above the body text
	void showFloatingInfo(QWidget* src, const QString& title, const QString& body, FloatingPosition position);

	/// Hides the floating window
	void hide();

private:
	FloatingInfoView* m_InfoView;

	/// Reposition this floating view to near the src widget
	void reposition(QWidget* src, FloatingPosition position);
};

#endif // FLOATINGTOOLTIPVIEW_H