#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include <QtCore>

const int DEFAULT_CANVAS_WIDTH = 32;
const int DEFAULT_CANVAS_HEIGHT = 32;

class ProjectModel : public QObject {
public:
	explicit ProjectModel();
	explicit ProjectModel(int width, int height);
	explicit ProjectModel(QString& projectPath);
	~ProjectModel();

	QString path();
	bool saved();
	QImage* surface();

	void setUnsaved();

private:
	QString m_Path;
	bool m_Saved;
	// QPoint m_ScrollPos;

	// QImage* m_Overlay; // These go in view?
	// QImage* m_Buffer;
	QImage* m_Surface;
};

#endif // PROJECTMODEL_H