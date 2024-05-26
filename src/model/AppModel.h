#ifndef APPMODEL_H
#define APPMODEL_H

#include <QtCore>
#include "ProjectModel.h"
#include "utils/Nullable.h"
#include "model/tools/AbstractTool.h"
#include <QColor>
#include "ui/components/floating/FloatingView.h"

class AppModel : public QObject {
	Q_OBJECT

public:
	explicit AppModel();
	~AppModel() override;

	/// Retrieves the list of projects contained in the AppModel
    const QList<ProjectModel*>& projects() const;

	/// Retrieves the current project - This may be null so is wrapped in a Nullable to force handling of this
    Nullable<ProjectModel> currProject() const;

    QColor primaryColour() const;
    QColor secondaryColour() const;

    const QList<AbstractTool*>& availableTools() const;
    AbstractTool *currentTool() const;

public slots: // TODO: How many of these are actually used as slots? Can change some to just functions for sure
	/// Creates a new project with the supplied settings. Emits projectAdded
	void newProject(int width, int height);

	/// Opens an image file (any supported by QImage), turns the format into ARGB32, and creates a project with that image.
	/// Emits projectAdded
	void openProject(QString& path);

	/// Deletes and removes the supplied project from the AppModel
	void closeProject(ProjectModel* project);

	/// Call this to set the current project to the supplied one, before emitting the currProjectUpdated signal.
	/// Also useful for simply informing this AppModel of updates to the current project
	void updateCurrProject(Nullable<ProjectModel> project);

	/// Changes the current tool to the one passed in
	void changeTool(AbstractTool* tool);

signals:
	/// Emitted when a project (supplied) is added to the AppModel. Emitted by newProject, openProject
	void projectAdded(ProjectModel* project);

	/// Emitted when a project (supplied) is updated or set as the current project. Emitted by updateCurrProject
	void currProjectUpdated(Nullable<ProjectModel> project);

	/// Emitted when the current tool is changed
	void toolChanged(AbstractTool* newTool);

	/// Emitted when a widget wants to display some floating information to the user
	void floatingInfoRequested(QWidget* src, const QString& title, const QString& body, FloatingPosition position);

	/// Emitted when a widget wants to no longer display any floating windows
	void hideFloating();

private:
	QList<ProjectModel*> m_Projects;
	Nullable<ProjectModel> m_CurrProject;

    QList<AbstractTool*> m_AvailableTools;
    AbstractTool* m_CurrentTool;

	QColor m_PrimaryColour;
	QColor m_SecondaryColour;
};

#endif // APPMODEL_H
