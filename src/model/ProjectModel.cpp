#include "ProjectModel.h"
#include <QImage>

const int DEFAULT_WIDTH = 32;
const int DEFAULT_HEIGHT = 32;

ProjectModel::ProjectModel() : QObject(), m_Path("unsaved"), m_Saved(false), m_Surface(new QImage(DEFAULT_WIDTH, DEFAULT_HEIGHT, QImage::Format_ARGB32)) {
	m_Surface->fill(Qt::GlobalColor::transparent);
}

ProjectModel::ProjectModel(int width, int height) : QObject(), m_Path("unsaved"), m_Saved(false), m_Surface(new QImage(width, height, QImage::Format_ARGB32)) {
	m_Surface->fill(Qt::GlobalColor::transparent);
}

ProjectModel::~ProjectModel() {
	delete m_Surface;
}

QString ProjectModel::path() {
	return this->m_Path;
}

bool ProjectModel::saved() {
	return this->m_Saved;
}

QImage* ProjectModel::surface() {
	return this->m_Surface;
}

void ProjectModel::setUnsaved() {
	this->m_Saved = false;
}