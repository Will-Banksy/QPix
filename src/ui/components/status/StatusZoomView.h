#ifndef STATUSZOOMVIEW_H
#define STATUSZOOMVIEW_H

#include <QAbstractSpinBox>
#include "model/AppModel.h"

class StatusZoomView : public QAbstractSpinBox {
	Q_OBJECT

public:
	explicit StatusZoomView(AppModel* model);
	~StatusZoomView() override;

	QValidator::State validate(QString& input, int& pos) const override;
	void stepBy(int steps) override;

public slots:
	void updateValueFrom(Nullable<ProjectModel> project);

protected:
	QAbstractSpinBox::StepEnabled stepEnabled() const override;

private:
	AppModel* m_Model;
};

#endif // STATUSZOOMVIEW_H