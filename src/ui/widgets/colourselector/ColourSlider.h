#ifndef COLOURSLIDER_H
#define COLOURSLIDER_H

const int COLSLIDER_DEFAULT_LENGTH = 255;

#include <QAbstractSlider>

class ColourSlider : public QAbstractSlider {
	Q_OBJECT

public:
	explicit ColourSlider(QImage* bgImg, Qt::Orientation orientation = Qt::Orientation::Horizontal, QWidget* parent = nullptr);
	~ColourSlider() override;

	/// Sets the preferred length along the main axis (dependent on orientation). This is by default 255.
	///
	/// Note that this sets the preferred length of the filled section of the widget, which has a 2px margin,
	/// so the actual widget length will be `m_PrefLength + 4`
	void setPreferredLength(int length);

protected:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	QSize sizeHint() const override;

private:
	static QPixmap* s_TransparentBackground;

	QImage* m_BgImg; // does not own
	int m_PrefLength;

	void setFromClick(const QPoint& mousePos);
};

#endif // COLOURSLIDER_H