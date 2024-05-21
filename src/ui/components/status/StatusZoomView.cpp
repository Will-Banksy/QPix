#include "StatusZoomView.h"
#include <QDoubleSpinBox>
#include <QLineEdit>

StatusZoomView::StatusZoomView(AppModel* model) : QAbstractSpinBox(), m_Model(model) {
	this->setFixedWidth(80);
	this->setReadOnly(true);

	this->updateValueFrom(model->currProject());

	connect(model, &AppModel::currProjectUpdated, this, &StatusZoomView::updateValueFrom);
}

StatusZoomView::~StatusZoomView() {
}

QValidator::State StatusZoomView::validate(QString& input, int& pos) const {
	return QValidator::State::Acceptable;
}

void StatusZoomView::stepBy(int steps) {
	if(m_Model->currProject().isNotNull()) {
		m_Model->currProject().unwrap()->stepZoom(steps > 0, nullptr);
	}
}

void StatusZoomView::updateValueFrom(Nullable<ProjectModel> project) {
	if(project.isNotNull()) {
		this->lineEdit()->setText(QString::number(project.unwrap()->zoom() * 100));
	} else {
		this->lineEdit()->setText("100");
	}
	this->update();
}

QAbstractSpinBox::StepEnabled StatusZoomView::stepEnabled() const {
	if(m_Model->currProject().isNull()) {
		return QAbstractSpinBox::StepNone;
	} else {
		if(m_Model->currProject().unwrap()->zoom() == ZOOM_FACTORS[0]) {
			return QAbstractSpinBox::StepUpEnabled;
		} else if(m_Model->currProject().unwrap()->zoom() == ZOOM_FACTORS[NUM_ZOOM_FACTORS - 1]) {
			return QAbstractSpinBox::StepDownEnabled;
		} else {
			return QAbstractSpinBox::StepUpEnabled | QAbstractSpinBox::StepDownEnabled;
		}
	}
}