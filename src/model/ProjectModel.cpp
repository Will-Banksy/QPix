#include "ProjectModel.h"
#include <QImage>
#include <filesystem>

namespace fs = std::filesystem;

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

ProjectModel::ProjectModel(const QString& path) : QObject(),
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
	return m_Path;
}

void ProjectModel::setPath(const QString& path, bool isRealPath) {
	m_Path = path;
	m_SavedAs = isRealPath;
}

const QString ProjectModel::displayName() const {
	// TODO: Note that files with the same name but different directories will have the same display name.
	//       Perhaps fix - This would require recalculating all display names every new project, and giving
	//       more context to this function
	return ProjectModel::displayNameOf(m_Path, m_SavedAs);
}

const QString ProjectModel::displayNameOf(const QString& path, bool isRealPath) {
	// TODO: Note that files with the same name but different directories will have the same display name.
	//       Perhaps fix - This would require recalculating all display names every new project, and giving
	//       more context to this function
	if(isRealPath) {
		fs::path stdpath = fs::path(path.toStdString());
		fs::path name = stdpath.filename();
		return QString::fromStdString(name.string());
	} else {
		return path;
	}
}

bool ProjectModel::hasPath() const {
	return m_SavedAs;
}

bool ProjectModel::saved() const {
	return m_Saved;
}

QImage& ProjectModel::surface() const {
	return *m_Surface;
}

QImage& ProjectModel::overlay() const {
	return *m_Overlay;
}

QImage& ProjectModel::buffer() const {
	return *m_Buffer;
}

float ProjectModel::zoom() const {
	return m_Zoom;
}

void ProjectModel::setZoom(float newZoom, QPointF* zoomOrigin) {
	emit zoomUpdated(m_Zoom, newZoom, zoomOrigin);
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

void ProjectModel::setSaved() {
	emit this->savedStateUpdated(true);
	m_Saved = true;
	emit this->anythingUpdated();
}

void ProjectModel::setUnsaved() {
	emit this->savedStateUpdated(false);
	m_Saved = false;
	emit this->anythingUpdated();
}

void ProjectModel::commitBuffer() {
	memcpy(m_Surface->bits(), m_Buffer->bits(), m_Buffer->width() * m_Buffer->height() * 4);
	this->setUnsaved();
}

void ProjectModel::revertBuffer() {
	memcpy(m_Buffer->bits(), m_Surface->bits(), m_Buffer->width() * m_Buffer->height() * 4);
}

bool ProjectModel::save() { // TODO: We really need better error handling here, and probably elsewhere. Problem is, error handling in C++ is a pain in the ass...
	if(this->hasPath()) {
		bool ret = m_Surface->save(m_Path, "png");
		if(ret) {
			this->setSaved();
		}
		return ret;
	} else {
		return false;
	}
}
