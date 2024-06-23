#include "ColourSlider.h"
#include <QPainter>
#include <iostream>
#include <QPainterPath>
#include "utils/Utils.h"

QPixmap* ColourSlider::s_TransparentBackground = nullptr;

ColourSlider::ColourSlider(QImage* bgImg, Qt::Orientation orientation, QWidget* parent) : QSlider(orientation, parent), m_BgImg(bgImg) {
	if(!s_TransparentBackground) {
		s_TransparentBackground = new QPixmap(":/data/canvas_bg_dark.png");
	}

	// TODO: Why are the colour sliders not being obeyed when they set their minimum sizes?
}

ColourSlider::~ColourSlider() {
}

void ColourSlider::paintEvent(QPaintEvent* event) {
	QPainter painter = QPainter(this);
	painter.setRenderHint(QPainter::RenderHint::Antialiasing, true);

	QPainterPath roundedPath = QPainterPath();
	roundedPath.addRoundedRect(QRect(0, 0, this->width(), this->height()), 2, 2);
	painter.setClipPath(roundedPath);

	painter.drawTiledPixmap(QRect(0, 0, this->width(), this->height()), *s_TransparentBackground);
	painter.drawImage(QRect(0, 0, this->width(), this->height()), *m_BgImg);

	switch(this->orientation()) {
		case Qt::Orientation::Horizontal: {
			int x = utils::map(this->value(), this->minimum(), this->maximum(), 0, this->width());
			painter.setPen(QColorConstants::White);
			painter.drawLine(x, 0, x, this->height());
			break;
		}
		case Qt::Orientation::Vertical: {
			int y = utils::map(this->value(), this->minimum(), this->maximum(), 0, this->height());
			painter.setPen(QColorConstants::White);
			painter.drawLine(0, y, this->width(), y);
			break;
		}
	}

	QSlider::paintEvent(event); // TODO: Ideally we don't want to overlay the QSlider appearance just to have something usable - Need to make a better, styled handle where the actual value is at the centre of that handle
}
