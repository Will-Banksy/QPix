#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include <QtCore>

const int DEFAULT_CANVAS_WIDTH = 32;
const int DEFAULT_CANVAS_HEIGHT = 32;

const int NUM_ZOOM_FACTORS = 21;
const int DEFAULT_ZOOM_FACTOR_IDX = 8;
const float ZOOM_FACTORS[NUM_ZOOM_FACTORS] = {
	0.01, 0.02, 0.04, 0.08, 0.16, 0.32, 0.5, 0.75,
	1.0,
	1.5, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 12.0, 16.0, 32.0, 64.0
};

class ProjectModel : public QObject { // TODO: Perhaps create project types, or have a tab not directly correspond to a project... or idk, some way to have different types of tabs
	Q_OBJECT

public:
	explicit ProjectModel();
	explicit ProjectModel(int width, int height);
	explicit ProjectModel(QString& projectPath);
	~ProjectModel() override;

	///	Returns the absolute path to the file for this project, or "unsaved" is there is no path.
	/// This should not be used definitively however - Use hasPath() to determine whether this project
	/// corresponds to a file
    const QString& path() const;

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

	/// Zoom in to the next zoom factor in ZOOM_FACTORS (direction depending on zoomIn value), saturating on bounds
	void stepZoom(bool zoomIn, QPointF* zoomOrigin = nullptr);

	/// Asserts that the most recent change has not been saved (i.e. sets m_Saved to false)
	void setUnsaved();

	/// Commits the contents of the drawing buffer to the surface
	void commitBuffer();
	/// Reverts changes to the drawing buffer by copying the surface
	void revertBuffer();

signals:
	/// Emitted when anything is updated. Emitted by setZoom and setUnsaved
	void anythingUpdated();
	/// Emitted when the zoom is updated. Emitted by setZoom
	void zoomUpdated(float oldZoom, float newZoom, QPointF* zoomOrigin = nullptr);

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
	/// For displaying action previews
	QImage* m_Overlay;
	/// Buffer on top of surface - the actual stored content
	QImage* m_Buffer;
};

#endif // PROJECTMODEL_H
