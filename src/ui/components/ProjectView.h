#ifndef PROJECTVIEW_H
#define PROJECTVIEW_H

#include <QGraphicsView>
#include "model/ProjectModel.h"
#include "model/AppModel.h"
#include "canvas/ProjectCanvasView.h"

class ProjectView : public QGraphicsView {
	Q_OBJECT

public:
	explicit ProjectView(ProjectModel* model, AppModel* appModel);
	~ProjectView() override;

	ProjectModel* model();

public slots:
	void setZoom(float oldZoom, float newZoom, QPointF* zoomOrigin);

protected:
	void wheelEvent(QWheelEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    // void paintEvent(QPaintEvent* event) override;

private:
	ProjectModel* m_Model;
	AppModel* m_AppModel;

	ProjectCanvasView* m_CanvasView;

	bool m_MouseDown;
	Qt::MouseButton m_MouseButton;
	bool m_HasMovedMouse;
	bool m_IgnoreRelease;
	QPoint m_PrevCanvasCoord;

	void updateScrollMargins(float withZoom);
	QPoint mapToCanvas(const QPoint& pt);
};

#endif // PROJECTVIEW_H
