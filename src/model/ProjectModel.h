#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include <QtCore>

class ProjectModel : public QObject {
public:
	explicit ProjectModel();
	explicit ProjectModel(int width, int height);
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