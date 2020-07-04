#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsItem>

/**
 * @todo write docs
 */
class Canvas : public QGraphicsItem {
	public:
		explicit Canvas();
		~Canvas();
		QImage* surface;
		QImage* overlay;

	protected:
		void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
		QRectF boundingRect() const override;

	private:
		QPixmap* background;
};

#endif // CANVAS_H
