#include "canvas.h"
#include <QPainter>

Canvas::Canvas() : QGraphicsItem() {
	surface = new QImage(32, 32, QImage::Format_ARGB32);
	surface->fill(0x00000000);

	overlay = new QImage(surface->width(), surface->height(), QImage::Format_ARGB32);
	overlay->fill(0x00000000);

	buffer = new QImage(*surface);

	background = new QPixmap(":/Transparency-Dark.png");
}

Canvas::~Canvas() {
	delete surface;
	delete overlay;
	delete buffer;
	delete background;
}

void Canvas::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
	QRect bounds = QRect(0, 0, surface->width(), surface->height());
	painter->drawTiledPixmap(bounds, *background);

	painter->drawImage(0, 0, *buffer);

// 	painter->drawImage(0, 0, *overlay);
}

QRectF Canvas::boundingRect() const {
	return QRectF(0, 0, surface->width(), surface->height());
}

void Canvas::commit() {
	memcpy(surface->bits(), buffer->bits(), surface->width() * surface->height() * 4);
	update();
}

void Canvas::revert() {
	memcpy(buffer->bits(), surface->bits(), surface->width() * surface->height() * 4);
}