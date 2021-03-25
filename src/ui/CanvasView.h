#ifndef CANVASVIEW_H
#define CANVASVIEW_H

#include <QGraphicsView>

class CanvasView : public QGraphicsView {
	Q_OBJECT

public:
	CanvasView(QWidget* parent = nullptr);
	~CanvasView();

	void updateScrollMargins();

protected:
	void wheelEvent(QWheelEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

private:
	bool mouseDown;
	bool hasMovedMouse;
	bool ignoreRelease;

	QPoint prevCanvasCoord;

	QPoint mapToCanvas(const QPoint& pt);
	QPoint mapToCanvas(int x, int y);
	QPoint mapFromCanvas(const QPoint& pt);
	QPoint mapFromCanvas(int x, int y);

public:
	double scaleAmt; // Keeps track of the current scale
};

#endif // CANVASVIEW_H
