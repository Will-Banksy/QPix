#ifndef APPMODEL_H
#define APPMODEL_H

#include <QtCore>
#include "ProjectModel.h"

class AppModel : public QObject {
	Q_OBJECT

public:
	explicit AppModel();
	~AppModel();

	QList<ProjectModel*>* projects();

public slots:
	void newProject();
	void closeProject(ProjectModel* project);

signals:
	void projectAdded(ProjectModel* project);

private:
	QList<ProjectModel*> m_Projects;
};

#endif // APPMODEL_H