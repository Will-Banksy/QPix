#include "StatusZoomView.h"
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QComboBox>

StatusZoomView::StatusZoomView(AppModel* model) : QWidget(), m_Model(model), m_ZoomComboBox(new QComboBox()) {
	QLabel* label = new QLabel("Zoom:");

	m_ZoomComboBox->setEditable(false);
	for(int i = 0; i < NUM_ZOOM_FACTORS; i++) {
		m_ZoomComboBox->insertItem(i, QString::number(ZOOM_FACTORS[i] * 100) + "%", ZOOM_FACTORS[i]);
	}

	this->setIndexFromProject(m_Model->currProject());

	connect(m_ZoomComboBox, &QComboBox::currentIndexChanged, this, &StatusZoomView::setZoomFromIndex);
	connect(model, &AppModel::currProjectUpdated, this, &StatusZoomView::setIndexFromProject);

	QHBoxLayout* layout = new QHBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);

	layout->addWidget(label);
	layout->addWidget(m_ZoomComboBox);

	this->setLayout(layout);
}

StatusZoomView::~StatusZoomView() {
}

void StatusZoomView::setZoomFromIndex(int index) {
	if(this->m_Model->currProject().isNotNull()) {
		this->m_Model->currProject().unwrap()->setZoom(ZOOM_FACTORS[index]);
	}
}

void StatusZoomView::setIndexFromProject(Nullable<ProjectModel> project) {
	if(project.isNotNull()) {
		this->m_ZoomComboBox->setEnabled(true);
		for(int i = 0; i < NUM_ZOOM_FACTORS; i++) {
			if(ZOOM_FACTORS[i] == project.unwrap()->zoom()) {
				this->m_ZoomComboBox->setCurrentIndex(i);
				return;
			}
		}
		assert(false);
	} else {
		this->m_ZoomComboBox->setCurrentIndex(DEFAULT_ZOOM_FACTOR_IDX);
		this->m_ZoomComboBox->setEnabled(false);
	}
}