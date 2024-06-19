#include "ColourButton.h"
#include <QPainter>

ColourButton::ColourButton(const QColor& colour) : QToolButton(), m_Colour(colour) {
	this->setMinimumSize(32, 32); // TODO: Derive a better size... Be nice if it'd be the same size as other tool buttons
}

ColourButton::~ColourButton() {
}

void ColourButton::updateColour(const QColor& colour) {
	m_Colour = colour;

	this->update();
}

void ColourButton::paintEvent(QPaintEvent* event) { // TODO: Transparency texture drawn underneath colour - Bit much to load the whole canvas transparency texture so how do we do this
	QToolButton::paintEvent(event);

	QRect colourBounds = QRect(0, 0, this->width(), this->height()).adjusted(4, 4, -4, -4);

	QPainter painter = QPainter(this);

	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::PenStyle::NoPen);
	painter.setBrush(m_Colour);
	painter.drawRoundedRect(colourBounds, 2, 2);
}
