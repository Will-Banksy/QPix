#ifndef STATUSZOOMVIEW_H
#define STATUSZOOMVIEW_H

#include <QAbstractSpinBox>
#include "model/AppModel.h"

class QComboBox;

class StatusZoomView : public QWidget {
	Q_OBJECT

public:
	explicit StatusZoomView(AppModel* model);
	~StatusZoomView() override;

public slots:
	void setZoomFromIndex(int index);
	void setIndexFromProject(Nullable<ProjectModel> project);

private:
	AppModel* m_Model;
	QComboBox* m_ZoomComboBox;
};

#endif // STATUSZOOMVIEW_H