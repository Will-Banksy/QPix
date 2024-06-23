#include "ColourButton.h"
#include <QPainter>
#include <QPainterPath>

QPixmap* ColourButton::s_TransparentBackground = nullptr;

ColourButton::ColourButton(const QColor& colour) : QToolButton(), m_Colour(colour) {
	if(!s_TransparentBackground) {
		s_TransparentBackground = new QPixmap(":/data/canvas_bg_dark.png");
	}

	this->setMinimumSize(32, 32); // TODO: Derive a better size... Be nice if it'd be the same size as other tool buttons
}

ColourButton::~ColourButton() {
}

void ColourButton::updateColour(const QColor& colour) {
	m_Colour = colour;

	this->update();
}

void ColourButton::paintEvent(QPaintEvent* event) {
	QToolButton::paintEvent(event);

	QRect colourBounds = QRect(0, 0, this->width(), this->height()).adjusted(4, 4, -4, -4);

	QPainter painter = QPainter(this);
	painter.setRenderHint(QPainter::RenderHint::Antialiasing, true);

	// Rounded clip
	QPainterPath roundedPath = QPainterPath();
	roundedPath.addRoundedRect(colourBounds, 2, 2);
	painter.setClipPath(roundedPath);

	painter.drawTiledPixmap(colourBounds, *s_TransparentBackground);

	painter.setPen(Qt::PenStyle::NoPen);
	painter.setBrush(m_Colour);
	painter.drawRect(colourBounds);
}
