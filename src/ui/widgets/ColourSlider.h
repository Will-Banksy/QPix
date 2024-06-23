#ifndef COLOURSLIDER_H
#define COLOURSLIDER_H

#include <QSlider>

class ColourSlider : public QSlider {
	Q_OBJECT

public:
	explicit ColourSlider(QImage* bgImg, Qt::Orientation orientation = Qt::Orientation::Horizontal, QWidget* parent = nullptr);
	~ColourSlider() override;

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	static QPixmap* s_TransparentBackground;

	QImage* m_BgImg; // does not own
};

#endif // COLOURSLIDER_H