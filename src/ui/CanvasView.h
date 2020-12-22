#ifndef CANVASVIEW_H
#define CANVASVIEW_H

#include <QGraphicsView>

class CanvasView : public QGraphicsView {
	Q_OBJECT

public:
	CanvasView(QWidget* parent = nullptr);
	~CanvasView();

protected:
	void wheelEvent(QWheelEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

private:
	bool mouseDown;
	bool hasMovedMouse;
	bool ignoreRelease;

	double scaleAmt; // Keeps track of the current scale

	QPoint prevCanvasCoord;
};

#endif // CANVASVIEW_H
