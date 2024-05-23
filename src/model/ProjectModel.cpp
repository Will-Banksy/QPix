#include "ProjectModel.h"
#include <QImage>

ProjectModel::ProjectModel() : QObject(),
	m_Path("unsaved"),
	m_Saved(false),
	m_SavedAs(false),
	m_Zoom(1.0),
	m_Surface(new QImage(DEFAULT_CANVAS_WIDTH, DEFAULT_CANVAS_HEIGHT, QImage::Format_ARGB32)),
	m_Overlay(new QImage(DEFAULT_CANVAS_WIDTH, DEFAULT_CANVAS_HEIGHT, QImage::Format_ARGB32)),
	m_Buffer(new QImage(*m_Surface)) {
	m_Surface->fill(QColorConstants::Transparent);
	m_Overlay->fill(QColorConstants::Transparent);
	m_Buffer->fill(QColorConstants::Transparent);
	// for(int i = 0; i < m_Surface->width(); i++) {
	// 	for(int j = 0; j < m_Surface->height(); j++) {
	// 		m_Surface->setPixel(i, j,
	// 			utils::Colour::toIntAHSL(100, i, j, i % 2 == 0 || j % 2 == 0 ? 0 : 50)
	// 		);
	// 	}
	// }
}

ProjectModel::ProjectModel(int width, int height) : QObject(),
	m_Path("unsaved"),
	m_Saved(false),
	m_SavedAs(false),
	m_Zoom(1.0),
	m_Surface(new QImage(width, height, QImage::Format_ARGB32)),
	m_Overlay(new QImage(width, height, QImage::Format_ARGB32)),
	m_Buffer(new QImage(*m_Surface)) {
	m_Surface->fill(QColorConstants::Transparent);
	m_Overlay->fill(QColorConstants::Transparent);
	m_Buffer->fill(QColorConstants::Transparent);
}

ProjectModel::ProjectModel(QString& path) : QObject(),
	m_Path(path),
	m_Saved(true),
	m_SavedAs(true),
	m_Zoom(1.0),
	m_Surface(new QImage(path)),
	m_Overlay(new QImage(m_Surface->width(), m_Surface->height(), QImage::Format_ARGB32)),
	m_Buffer(new QImage(*m_Surface)) {
	m_Overlay->fill(QColorConstants::Transparent);
	m_Surface->convertTo(QImage::Format_ARGB32);
}

ProjectModel::~ProjectModel() {
	delete m_Surface;
	delete m_Overlay;
	delete m_Buffer;
}

const QString& ProjectModel::path() const {
	return this->m_Path;
}

bool ProjectModel::hasPath() const {
	return m_SavedAs;
}

bool ProjectModel::saved() const {
	return this->m_Saved;
}

QImage& ProjectModel::surface() const {
	return *this->m_Surface;
}

QImage& ProjectModel::overlay() const {
	return *this->m_Overlay;
}

QImage& ProjectModel::buffer() const {
	return *this->m_Buffer;
}

float ProjectModel::zoom() const {
	return this->m_Zoom;
}

void ProjectModel::setZoom(float newZoom, QPointF* zoomOrigin) {
	emit zoomUpdated(this->m_Zoom, newZoom, zoomOrigin);
	m_Zoom = newZoom;
	emit anythingUpdated();
}

void ProjectModel::stepZoom(bool zoomIn, QPointF* zoomOrigin) {
	// Calculate the new zoom value
	int zoomI = 0;
	for(int i = 0; i < NUM_ZOOM_FACTORS; i++) {
		if(ZOOM_FACTORS[i] == m_Zoom) {
			zoomI = i;
			break;
		}
	}
	float newZoom = 0;
	if(zoomIn) {
		newZoom = ZOOM_FACTORS[zoomI + 1 >= NUM_ZOOM_FACTORS ? NUM_ZOOM_FACTORS - 1 : zoomI + 1];
	} else {
		if(zoomI != 0) {
			newZoom = ZOOM_FACTORS[zoomI - 1];
		} else {
			newZoom = m_Zoom;
		}
	}
	this->setZoom(newZoom, zoomOrigin);
}

void ProjectModel::setUnsaved() {
	this->m_Saved = false;
	emit anythingUpdated();
}

void ProjectModel::commitBuffer() {
	memcpy(m_Surface->bits(), m_Buffer->bits(), m_Buffer->width() * m_Buffer->height() * 4);
	this->setUnsaved();
}

void ProjectModel::revertBuffer() {
	memcpy(m_Buffer->bits(), m_Surface->bits(), m_Buffer->width() * m_Buffer->height() * 4);
}