#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include <QtCore>

const int DEFAULT_CANVAS_WIDTH = 32;
const int DEFAULT_CANVAS_HEIGHT = 32;

// TODO: Maybe these should be changeable at runtime, in a config with other settings that can be reloaded...? Some people might not like the power of 2 based zoom factors, I need to consider... or is that likely to be the case?
const int NUM_ZOOM_FACTORS = 23;
const int DEFAULT_ZOOM_FACTOR_IDX = 8;
const float ZOOM_FACTORS[NUM_ZOOM_FACTORS] = {
	0.01, 0.015625, 0.03125, 0.0625, 0.125, 0.25, 0.5, 0.75,
	1.0,
	1.5, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 12.0, 16.0, 24.0, 32.0, 48.0, 64.0
};

class ProjectModel : public QObject { // TODO: Perhaps create project types, or have a tab not directly correspond to a project... or idk, some way to have different types of tabs
	Q_OBJECT

public:
	explicit ProjectModel();
	explicit ProjectModel(int width, int height);
	explicit ProjectModel(const QString& projectPath);
	~ProjectModel() override;

	///	Returns the absolute path to the file for this project, or "unsaved" is there is no path.
	/// This should not be used definitively however - Use hasPath() to determine whether this project
	/// corresponds to a file
    const QString& path() const;

	/// Sets the path for this project. Pass false for isRealPath for this project to be "detached" from a file
	void setPath(const QString& path, bool isRealPath = true);

	/// The short name that should be used to identify this file. This is usually the file name, or
	/// "unsaved" if no path
	const QString displayName() const;

	/// The short name that should be used to identify the passed-in file path. This is usually the file name, or
	/// the path as-is if it is not a real path
	const static QString displayNameOf(const QString& path, bool isRealPath);

	/// Returns whether this project corresponds to a file
    bool hasPath() const;

	/// Returns whether the most recent change to this project has been saved
    bool saved() const;

	/// Returns the current drawable surface
    QImage& surface() const;

	/// Returns the overlay image
	QImage& overlay() const;

	/// Returns the drawing buffer for the current drawable surface
	QImage& buffer() const;

	/// Returns the zoom factor, that necessarily lies in the ZOOM_FACTORS array
    float zoom() const;

	/// Update the zoom factor, zooming around zoomOrigin (in viewport coordinates) if specified. Emits zoomUpdated
	void setZoom(float newZoom, QPointF* zoomOrigin = nullptr);

	/// Zoom in to the next zoom factor in ZOOM_FACTORS (direction depending on zoomIn value), saturating on bounds. Emits zoomUpdated
	void stepZoom(bool zoomIn, QPointF* zoomOrigin = nullptr);

	/// Sets that the most recent change has been saved (i.e. sets m_Saved to true)
	void setSaved();

	/// Sets that the most recent change has not been saved (i.e. sets m_Saved to false)
	void setUnsaved();

	/// Commits the contents of the drawing buffer to the surface
	void commitBuffer();

	/// Reverts changes to the drawing buffer by copying the surface
	void revertBuffer();

	/// Saves the file content to the file path, if it has one. Returns true on successful save, false on failed save
	/// or the case that this project does not have a path
	bool save();

signals:
	/// Emitted when anything is updated. Emitted along with all other updated signals
	void anythingUpdated();

	/// Emitted when the zoom is updated. Emitted by setZoom/stepZoom
	void zoomUpdated(float oldZoom, float newZoom, QPointF* zoomOrigin = nullptr);

	/// Emitted when the path is updated. Emitted by setPath
	void pathUpdated(const QString& path);

	/// Emitted when the project is saved or changed (becoming unsaved). Emitted by save, setUnsaved & commitBuffer
	void savedStateUpdated(bool saved);

private:
	/// Path to corresponding file, or "unsaved"
	QString m_Path;
	/// Whether the most recent change has been saved
	bool m_Saved;
	/// Whether this project corresponds to a file
	bool m_SavedAs;
	float m_Zoom;

	/// The current drawable surface (// TODO: Decide what exactly to do for multi-layer support
	/// - Current thinking is to simply have a list of surfaces and this stays as the "current drawable surface")
	QImage* m_Surface;
	/// For displaying action previews (currently unused I think)
	QImage* m_Overlay;
	/// Buffer on top of surface - the actual stored content
	QImage* m_Buffer;
};

#endif // PROJECTMODEL_H
