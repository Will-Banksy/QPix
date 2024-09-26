#ifndef COLOURBOXSLIDER_H
#define COLOURBOXSLIDER_H

#include "../abstract/AbstractVariantSlider.h"

const int COLBOXSLIDER_DEFAULT_WIDTH = 255;
const int COLBOXSLIDER_DEFAULT_HEIGHT = 255;

class ColourBoxSlider : public AbstractVariantSlider {
public:
	explicit ColourBoxSlider(QImage* bgImg, QWidget* parent = nullptr);
	~ColourBoxSlider() override;

	/// Sets the preferred size. This is by default (255, 255).
	///
	/// Note that this sets the preferred size of the filled section of the widget, which has a 2px margin,
	/// so the actual widget size will be `(m_PrefLength + 4, m_PrefLength + 4)`
	void setPreferredSize(const QSize& size);

protected:
	bool validateValue(QVariant& value) override;
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	QSize sizeHint() const override;

private:
	QImage* m_BgImg; // does not own
	QSize m_PrefSize;

	void setFromClick(const QPoint& mousePos);
};

#endif // COLOURBOXSLIDER_H