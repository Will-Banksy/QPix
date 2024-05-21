#ifndef APPMODEL_H
#define APPMODEL_H

#include <QtCore>
#include "ProjectModel.h"
#include "utils/Nullable.h"

class AppModel : public QObject {
	Q_OBJECT

public:
	explicit AppModel();
	~AppModel() override;

	/// Retrieves the list of projects contained in the AppModel
	QList<ProjectModel*>* projects();

	/// Retrieves the current project - This may be null so is wrapped in a Nullable to force handling of this
	Nullable<ProjectModel> currProject();

public slots: // TODO: How many of these are actually used as slots? Can change some to just functions for sure
	/// Creates a new project with the supplied settings. Emits projectAdded
	void newProject(int width, int height);

	/// Opens an image file (any supported by QImage), turns the format into ARGB32, and creates a project with that image.
	/// Emits projectAdded
	void openProject(QString& path);

	/// Deletes and removes the supplied project from the AppModel
	void closeProject(ProjectModel* project);

	/// Call this to set the current project to the supplied one, after emitting the currProjectUpdated signal.
	/// Also useful for simply informing this AppModel of updates to the current project
	void updateCurrProject(Nullable<ProjectModel> project);

signals:
	/// Emitted when a project (supplied) is added to the AppModel. Emitted by newProject, openProject
	void projectAdded(ProjectModel* project);

	/// Emitted when a project (supplied) is updated or set as the current project. Emitted by updateCurrProject
	void currProjectUpdated(Nullable<ProjectModel> project);

private:
	QList<ProjectModel*> m_Projects;
	Nullable<ProjectModel> m_CurrProject;

	// int m_PrimaryColour;
	// int m_SecondaryColour;
};

#endif // APPMODEL_H