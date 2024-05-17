#include "ProjectCanvasView.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>

QPixmap* ProjectCanvasView::s_Background = nullptr;

ProjectCanvasView::ProjectCanvasView(ProjectModel* model, AppModel* appModel)
	: QGraphicsItem(), m_Model(model), m_AppModel(appModel),
	  m_Overlay(new QImage(m_Model->surface()->width(), m_Model->surface()->height(), QImage::Format_ARGB32)),
	  m_Buffer(new QImage(*m_Model->surface())) {

	if(!s_Background) {
		s_Background = new QPixmap(":/canvas_bg_dark.png");
	}

	m_Overlay->fill(Qt::GlobalColor::transparent);

	this->setFlags(ItemUsesExtendedStyleOption);
}

ProjectCanvasView::~ProjectCanvasView() {
	delete m_Overlay;
	delete m_Buffer;
}

void ProjectCanvasView::commit() {
	memcpy(m_Model->surface()->bits(), m_Buffer->bits(), m_Buffer->width() * m_Buffer->height() * 4);
	this->update();
	m_Model->setUnsaved();
}

void ProjectCanvasView::revert() {
	memcpy(m_Buffer->bits(), m_Model->surface()->bits(), m_Buffer->width() * m_Buffer->height() * 4);
}

// QSize ProjectCanvasView::imageBounds() {
// 	return QSize(m_Model->surface()->width(), m_Model->surface()->height());
// }

void ProjectCanvasView::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
	// painter->setClipRect(option->exposedRect);

	// Draw background, and then the buffer and overlay
	painter->drawTiledPixmap(QRect(0, 0, m_Buffer->width(), m_Buffer->height()), *s_Background);
	painter->drawImage(0, 0, *m_Buffer);
	painter->drawImage(0, 0, *m_Overlay);
}

QRectF ProjectCanvasView::boundingRect() const {
	return QRectF(0, 0, m_Model->surface()->width(), m_Model->surface()->height());
}