#include "ColourSlider.h"
#include <QPainter>
#include <iostream>
#include <QPainterPath>
#include <QMouseEvent>
#include "utils/Utils.h"

QPixmap* ColourSlider::s_TransparentBackground = nullptr;

ColourSlider::ColourSlider(QImage* bgImg, Qt::Orientation orientation, QWidget* parent) : QAbstractSlider(parent), m_BgImg(bgImg) {
	if(!s_TransparentBackground) {
		s_TransparentBackground = new QPixmap(":/data/canvas_bg_dark.png");
	}

	this->setOrientation(orientation);
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
			int x = utils::map(this->value(), this->minimum(), this->maximum(), 0, this->width() - 1);
			painter.setPen(QPen(QColorConstants::Black, 2));
			painter.setBrush(QColorConstants::White);
			painter.drawRoundedRect(x - 1, 0, 3, this->height(), 2, 2);
			break;
		}
		case Qt::Orientation::Vertical: {
			int y = utils::map(this->value(), this->minimum(), this->maximum(), 0, this->height() - 1);
			painter.setPen(QPen(QColorConstants::Black, 2));
			painter.setBrush(QColorConstants::White);
			painter.drawRoundedRect(0, y - 1, this->width(), 3, 2, 2);
			break;
		}
	}
}

void ColourSlider::mousePressEvent(QMouseEvent* event) {
	this->setFromClick(event->pos());
}

void ColourSlider::mouseMoveEvent(QMouseEvent* event) {
	this->setFromClick(event->pos());
}

void ColourSlider::setFromClick(const QPoint& mousePos) {
	int value = 0;
	bool value_set = false;
	if(this->orientation() == Qt::Orientation::Horizontal) {
		value = utils::map(mousePos.x(), 0, this->width(), this->minimum(), this->maximum());
		value_set = true;
	} else if(this->orientation() == Qt::Orientation::Vertical) {
		value = utils::map(mousePos.y(), 0, this->height(), this->minimum(), this->maximum());
		value_set = true;
	}

	assert(value_set);

	value = utils::constrain(value, this->minimum(), this->maximum());

	this->setValue(value);
}
