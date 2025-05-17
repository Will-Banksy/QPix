#include "ColourSlider.h"
#include <QPainter>
#include <iostream>
#include <QPainterPath>
#include <QMouseEvent>
#include "utils/Utils.h"

QPixmap* ColourSlider::s_TransparentBackground = nullptr;

ColourSlider::ColourSlider(QImage* bgImg, Qt::Orientation orientation, QWidget* parent) : QAbstractSlider(parent), m_BgImg(bgImg), m_PrefLength(COLSLIDER_DEFAULT_LENGTH) {
	if(!s_TransparentBackground) {
		s_TransparentBackground = new QPixmap(":/assets/canvas_bg_dark.png");
	}

	this->setOrientation(orientation);

	switch(this->orientation()) {
		case Qt::Orientation::Horizontal: {
			this->setMaximumHeight(20);
			break;
		}
		case Qt::Orientation::Vertical: {
			this->setMaximumWidth(20);
			break;
		}
	}
}

ColourSlider::~ColourSlider() {
}

void ColourSlider::setPreferredLength(int length) {
	m_PrefLength = length;
}

void ColourSlider::paintEvent(QPaintEvent* event) {
	QPainter painter = QPainter(this);
	painter.setRenderHint(QPainter::RenderHint::Antialiasing, true);

	painter.save();

	QPainterPath roundedPath = QPainterPath();
	roundedPath.addRoundedRect(QRect(2, 2, this->width() - 4, this->height() - 4), 2, 2);
	painter.setClipPath(roundedPath);

	QSize antiArtifactOffset = QSize(1 * (int)(this->orientation() == Qt::Orientation::Horizontal), 1 * (int)(this->orientation() == Qt::Orientation::Vertical));

	painter.drawTiledPixmap(QRect(2, 2, this->width() - 4 - antiArtifactOffset.width(), this->height() - 4 - antiArtifactOffset.height()), *s_TransparentBackground);
	painter.drawImage(QRect(2, 2, this->width() - 4, this->height() - 4), *m_BgImg);

	painter.restore();

	switch(this->orientation()) {
		case Qt::Orientation::Horizontal: {
			int x = utils::map(this->value(), this->minimum(), this->maximum(), 2, this->width() - 3);
			painter.setPen(QPen(QColorConstants::White, 2));
			painter.setBrush(QColorConstants::Transparent);
			painter.drawRoundedRect(x - 1, 0, 3, this->height(), 2, 2);
			break;
		}
		case Qt::Orientation::Vertical: {
			int y = utils::map(this->value(), this->minimum(), this->maximum(), 2, this->height() - 3);
			painter.setPen(QPen(QColorConstants::White, 2));
			painter.setBrush(QColorConstants::Transparent);
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

QSize ColourSlider::sizeHint() const {
	switch(this->orientation()) {
		case Qt::Orientation::Horizontal: {
			return QSize(m_PrefLength + 4, 20);
		}
		case Qt::Orientation::Vertical: {
			return QSize(20, m_PrefLength + 4);
		}
	}

	assert(false);
	return *(QSize*)nullptr;
}

void ColourSlider::setFromClick(const QPoint& mousePos) {
	int value = 0;
	bool value_set = false;
	if(this->orientation() == Qt::Orientation::Horizontal) {
		value = utils::map(mousePos.x(), 2, this->width() - 3, this->minimum(), this->maximum());
		value_set = true;
	} else if(this->orientation() == Qt::Orientation::Vertical) {
		value = utils::map(mousePos.y(), 2, this->height() - 3, this->minimum(), this->maximum());
		value_set = true;
	}

	assert(value_set);

	value = utils::constrain(value, this->minimum(), this->maximum());

	this->setValue(value);
}
