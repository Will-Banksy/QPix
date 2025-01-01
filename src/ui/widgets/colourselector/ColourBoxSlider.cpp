#include <QWidget>
#include "ColourBoxSlider.h"
#include <QPainter>
#include <QPainterPath>
#include "utils/Utils.h"
#include <QMouseEvent>

ColourBoxSlider::ColourBoxSlider(QImage* bgImg, QWidget* parent) : AbstractVariantSlider(QVariant(), QMetaType(QMetaType::QPoint)), m_BgImg(bgImg) {
}

ColourBoxSlider::~ColourBoxSlider() {
}

void ColourBoxSlider::setPreferredSize(const QSize& size) {
	m_PrefSize = size;
}

bool ColourBoxSlider::validateValue(QVariant& value) {
	// At the point of this function being called, I *think* that we should be good to just call .toPoint()

	QPoint pt = value.toPoint();
	int x = pt.x();
	int y = pt.y();

	int minX = this->minimum().toPoint().x();
	int minY = this->minimum().toPoint().y();

	int maxX = this->maximum().toPoint().x();
	int maxY = this->maximum().toPoint().y();

	int clampedX = utils::constrain(x, minX, maxX);
	int clampedY = utils::constrain(y, minY, maxY);

	if(clampedX != x || clampedY != y) {
		value = QVariant(QPoint(clampedX, clampedY));
	}

	return true;
}

void ColourBoxSlider::paintEvent(QPaintEvent* event) {
	QPainter painter = QPainter(this);
	painter.setRenderHint(QPainter::RenderHint::Antialiasing, true);

	painter.save();

	QPainterPath roundedPath = QPainterPath();
	roundedPath.addRoundedRect(QRect(2, 2, this->width() - 4, this->height() - 4), 2, 2);
	painter.setClipPath(roundedPath);

	painter.drawImage(QRect(2, 2, this->width() - 4, this->height() - 4), *m_BgImg);

	painter.restore();

	QPoint pt = this->value().toPoint();
	int x = utils::map(pt.x(), this->minimum().toPoint().x(), this->maximum().toPoint().x(), 2, this->width() - 4);
	int y = utils::map(pt.y(), this->minimum().toPoint().y(), this->maximum().toPoint().y(), 2, this->height() - 4);

	// Dynamically change the colour of the indicator depending on the surrounding colour lightness
	// Perceptual lightness would be better here but HSL lightness works alright, if not amazingly (i.e. white is hard to see against green or yellow)
	// Implemented a quick fix for making the marker easier to see against bright hues by just comapring the hue against a hardcoded "bright hues" range
	// and reducing the threshold for a black marker if it's a bright hue
	QColor colHere = m_BgImg->pixelColor(QPoint(qMin(x, m_BgImg->width() - 1), qMin(y, m_BgImg->height() - 1)));
	bool brightHue = colHere.hslHue() > 45 && colHere.hslHue() < 200; // BUG: Slight issue where the hue is returned as -1 on a monochromatic colour
	if(brightHue && colHere.lightness() > 90 || colHere.lightness() > 128) {
		painter.setPen(QPen(QColorConstants::Black, 2));
	} else {
		painter.setPen(QPen(QColorConstants::White, 2));
	}

	painter.setBrush(colHere);
	painter.drawEllipse(QPoint(x + 1, y + 1), 2, 2);
}

void ColourBoxSlider::mousePressEvent(QMouseEvent* event) {
	setFromClick(event->pos());
}

void ColourBoxSlider::mouseMoveEvent(QMouseEvent* event) {
	setFromClick(event->pos());
}

QSize ColourBoxSlider::sizeHint() const {
	// QPoint min = this->minimum().toPoint();
	// QPoint max = this->maximum().toPoint();
	// return QSize((max.x() - min.x()) + 4, (max.y() - min.y()) + 4);
	return m_PrefSize.grownBy(QMargins(2, 2, 2, 2));
}

void ColourBoxSlider::setFromClick(const QPoint& mousePos) {
	int mx = utils::map(mousePos.x(), 2, this->width() - 4, this->minimum().toPoint().x(), this->maximum().toPoint().x());
	int my = utils::map(mousePos.y(), 2, this->height() - 4, this->minimum().toPoint().y(), this->maximum().toPoint().y());

	this->setValue(QVariant(QPoint(mx, my)));
}