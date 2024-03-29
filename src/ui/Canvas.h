#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsItem>

class Project;

class Canvas : public QGraphicsItem {
	public:
		Canvas();
		~Canvas();

		/// This is NOT for drawing anything that needs to end up on the surface - This is used for previews of actions only
		QImage* overlay;
		/// If you want to draw to the surface, draw to this buffer and commit()
		QImage* buffer;

		Project* project;

		/// Commits changes to the buffer - basically writes the buffer to the image
		void commit();
		/// Reverts the buffer back to the image
		void revert();
		/// Clears the overlay
		void clearOverlay();

	protected:
		void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
		QRectF boundingRect() const override;

	private:
		static QPixmap* background;
		QImage* surface;
};

#endif // CANVAS_H
