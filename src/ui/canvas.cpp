#include "canvas.h"
#include <QPainter>

Canvas::Canvas() : QGraphicsItem() {
	surface = /*new QImage("/home/william-banks/Pictures/Saved/Adorable_SCP-173.png");*/new QImage(32, 32, QImage::Format_ARGB32);
	surface->fill(0x00000000);

	overlay = new QImage(surface->width(), surface->height(), QImage::Format_ARGB32);
	overlay->fill(0x00000000);

	background = new QPixmap(":/Transparency-Dark.png");
}

Canvas::~Canvas() {
	delete surface;
	delete overlay;
	delete background;
}

void Canvas::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
	QRect bounds = QRect(0, 0, surface->width(), surface->height());
	painter->drawTiledPixmap(bounds, *background);

	painter->drawImage(0, 0, *surface);

// 	painter->drawImage(0, 0, *overlay);
}

QRectF Canvas::boundingRect() const {
	return QRectF(0, 0, surface->width(), surface->height());
}