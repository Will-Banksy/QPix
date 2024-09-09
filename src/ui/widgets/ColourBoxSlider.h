#ifndef COLOURBOXSLIDER_H
#define COLOURBOXSLIDER_H

#include "abstract/AbstractVariantSlider.h"

class ColourBoxSlider : public AbstractVariantSlider {
public:
	explicit ColourBoxSlider(QImage* bgImg, QWidget* parent = nullptr);
	~ColourBoxSlider() override;

	// QSize sizeHintPublic() const;

protected:
	bool validateValue(QVariant& value) override;
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	QSize sizeHint() const override;

private:
	QImage* m_BgImg; // does not own

	void setFromClick(const QPoint& mousePos);
};

#endif // COLOURBOXSLIDER_H