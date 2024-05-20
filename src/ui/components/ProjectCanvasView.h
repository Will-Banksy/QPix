#ifndef PROJECTCANVASVIEW_H
#define PROJECTCANVASVIEW_H

#include <QGraphicsItem>
#include "model/ProjectModel.h"
#include "model/AppModel.h"

class ProjectCanvasView : public QGraphicsItem {
public:
	explicit ProjectCanvasView(ProjectModel* model, AppModel* appModel);
	~ProjectCanvasView() override;

	void commit();
	void revert();

protected:
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
	QRectF boundingRect() const override;

private:
	ProjectModel* m_Model;
	AppModel* m_AppModel;
	/// For displaying action previews
	QImage* m_Overlay;
	/// Buffer on top of surface - the actual stored content
	QImage* m_Buffer;
	/// Background texture
	static QPixmap* s_Background;
};

#endif // PROJECTCANVASVIEW_H