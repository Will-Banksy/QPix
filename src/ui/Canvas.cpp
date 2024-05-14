#include "Canvas.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStyleOptionGraphicsItem>
#include "utils/Helper.h"
#include "utils/Colour.h"
#include <iostream>
#include <Project.h>
#include <QGraphicsDropShadowEffect>

QPixmap* Canvas::background;

Canvas::Canvas() : QGraphicsItem() {
	surface = /*new QImage("/home/william-banks/Downloads/20210306_131503.jpg");*/new QImage(32, 32, QImage::Format_ARGB32);
// 	surface->convertTo(QImage::Format_ARGB32);
	surface->fill(utils::Colour::TRANSPARENT);

	overlay = new QImage(surface->width(), surface->height(), QImage::Format_ARGB32);
	overlay->fill(utils::Colour::TRANSPARENT);

	buffer = new QImage(*surface);

	if(!background) {
		background = new QPixmap(":/canvas_bg_dark.png");
	}

	// So we get the exposedRect (visible rectangle) as a QStyleOptionGraphicsItem parameter in paint
	setFlags(ItemUsesExtendedStyleOption);

	// Struggles when zoomed in, so won't use this. Or maybe use it depending on scale
// 	QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
// 	shadow->setBlurRadius(20);
// 	shadow->setColor(QColorConstants::Black);
// 	shadow->setOffset(0, 0);
// 	setGraphicsEffect(shadow);
}

Canvas::~Canvas() {
	delete surface;
	delete overlay;
	delete buffer;
}

// Hmm... Painting seems to actually be fairly intensive - or, at least, more than it should be. Change this, probably by only repainting the areas that need it?
// Actually, it doesn't seem TOO much worse than Aseprite, except when we're zoomed in, which is when things get a bit more so
// But now I've optimised the canvas painting... The problem must be the graphics scene painting :/
// Maybe if I do all the scaling transformations on this Canvas instead of on the graphics view, maybe this optimisation won't go unnoticed
void Canvas::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
// 	QRect visible = utils::expandRound(option->exposedRect);
// 	QPoint offset(visible.x(), visible.y());

	painter->setClipRect(option->exposedRect);

// 	painter->drawTiledPixmap(visible, *background, offset);
// 	painter->drawImage(visible.x(), visible.y(), *buffer, visible.x(), visible.y(), visible.width(), visible.height());
// 	painter->drawImage(visible.x(), visible.y(), *overlay, visible.x(), visible.y(), visible.width(), visible.height());
	painter->drawTiledPixmap(QRect(0, 0, buffer->width(), buffer->height()), *background);
	painter->drawImage(0, 0, *buffer);
	painter->drawImage(0, 0, *overlay);

// 	std::cout << "Repainting" << std::endl;
}

QRectF Canvas::boundingRect() const {
	return QRectF(0, 0, surface->width(), surface->height());
}

void Canvas::commit() {
	memcpy(surface->bits(), buffer->bits(), surface->width() * surface->height() * 4);
	update();
	project->setUnsaved();
}

void Canvas::revert() {
	memcpy(buffer->bits(), surface->bits(), surface->width() * surface->height() * 4);
}

void Canvas::clearOverlay() {
	overlay->fill(utils::Colour::TRANSPARENT);
}