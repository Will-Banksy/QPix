#ifndef FLOATINGMODALVIEW_H
#define FLOATINGMODALVIEW_H

#include <QWidget>
#include "../FloatingView.h"

class AppModel;

class FloatingModalView : public QWidget {
	Q_OBJECT

public:
	explicit FloatingModalView(QWidget* parent, AppModel* model);
	~FloatingModalView() override;

	/// Shows a colour select modal popup
	void showColourSelectPopup(QWidget* src, const QColor& colour, ColourChangeCallback callback, FloatingPosition position);

	/// Hides the popup
	void hide();

protected:
	void keyPressEvent(QKeyEvent* event) override;

private:
	AppModel* m_Model;
};

#endif // FLOATINGMODALVIEW_H