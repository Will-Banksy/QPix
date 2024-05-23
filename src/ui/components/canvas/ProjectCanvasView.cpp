#include "ProjectCanvasView.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <iostream>

QPixmap* ProjectCanvasView::s_Background = nullptr;

ProjectCanvasView::ProjectCanvasView(ProjectModel* model, AppModel* appModel) : QGraphicsItem(),
	m_Model(model), m_AppModel(appModel) {

	if(!s_Background) {
		s_Background = new QPixmap(":/canvas_bg_dark.png");
	}

	this->setFlags(ItemUsesExtendedStyleOption);
	this->setTransformOriginPoint(QPointF(0, 0));
}

ProjectCanvasView::~ProjectCanvasView() {
}

// void ProjectCanvasView::commit() {
// 	memcpy(m_Model->surface().bits(), m_Buffer->bits(), m_Buffer->width() * m_Buffer->height() * 4);
// 	this->update();
// 	m_Model->setUnsaved();
// }

// void ProjectCanvasView::revert() {
// 	memcpy(m_Buffer->bits(), m_Model->surface().bits(), m_Buffer->width() * m_Buffer->height() * 4);
// }

void ProjectCanvasView::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
	// painter->setClipRect(option->exposedRect);

	std::cout << "Canvas paint" << std::endl;

	// Draw background, and then the buffer and overlay
	painter->drawTiledPixmap(QRect(0, 0, m_Model->buffer().width(), m_Model->buffer().height()), *s_Background);
	painter->drawImage(0, 0, m_Model->buffer());
	painter->drawImage(0, 0, m_Model->overlay());
}

QRectF ProjectCanvasView::boundingRect() const {
	return QRectF(0, 0, m_Model->surface().width(), m_Model->surface().height());
}