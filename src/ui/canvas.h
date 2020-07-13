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
		QImage* buffer;

		// Commits changes to the buffer - basically writes the buffer to the image
		void commit();
		// Reverts the buffer back to the image
		void revert();

	protected:
		void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
		QRectF boundingRect() const override;

	private:
		QPixmap* background;
};

#endif // CANVAS_H
