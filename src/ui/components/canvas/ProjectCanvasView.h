#ifndef PROJECTCANVASVIEW_H
#define PROJECTCANVASVIEW_H

#include <QGraphicsItem>
#include "model/ProjectModel.h"
#include "model/AppModel.h"

class ProjectCanvasView : public QGraphicsItem {
public:
	explicit ProjectCanvasView(ProjectModel* model, AppModel* appModel);
	~ProjectCanvasView() override;

	// /// Commits the contents of the drawing buffer to the surface
	// void commit();
	// /// Reverts changes to the drawing buffer by copying the surface
	// void revert();

protected:
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
	QRectF boundingRect() const override;

private:
	ProjectModel* m_Model;
	AppModel* m_AppModel;

	/// Background texture
	static QPixmap* s_Background;
};

#endif // PROJECTCANVASVIEW_H