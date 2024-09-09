#ifndef COLOURSLIDER_H
#define COLOURSLIDER_H

#include <QAbstractSlider>

class ColourSlider : public QAbstractSlider {
	Q_OBJECT

public:
	explicit ColourSlider(QImage* bgImg, Qt::Orientation orientation = Qt::Orientation::Horizontal, QWidget* parent = nullptr);
	~ColourSlider() override;

protected:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	QSize sizeHint() const override;

private:
	static QPixmap* s_TransparentBackground;

	QImage* m_BgImg; // does not own

	void setFromClick(const QPoint& mousePos);
};

#endif // COLOURSLIDER_H