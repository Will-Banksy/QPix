#include "ColourSelectView.h"
#include "ui/widgets/ColourButton.h"
#include "model/AppModel.h"
#include "ui/layouts/FlowLayout.h"

ColourSelectView::ColourSelectView(AppModel* model) : QWidget(), m_Model(model) {
	// TODO: Layout: A box containing two colour tool buttons (custom widget) diagonally to each other, slightly overlapped

	// TODO: UI for changing the colour. Popup on colour button press, and aseprite-style selector always shown below/alongside buttons for selecting the primary colour

	FlowLayout* layout = new FlowLayout();

	QAction* primaryBtnAct = new QAction();
	QAction* secondaryBtnAct = new QAction();

	ColourButton* primaryBtn = new ColourButton(model->primaryColour());
	primaryBtn->setDefaultAction(primaryBtnAct);
	ColourButton* secondaryBtn = new ColourButton(model->secondaryColour());
	secondaryBtn->setDefaultAction(secondaryBtnAct);

	connect(model, &AppModel::primaryColourChanged, primaryBtn, &ColourButton::updateColour);
	connect(model, &AppModel::secondaryColourChanged, secondaryBtn, &ColourButton::updateColour);

	connect(primaryBtn, &ColourButton::triggered, this, [this, primaryBtn](QAction* action) {
		emit this->m_Model->modalColourSelectRequested(primaryBtn, this->m_Model->primaryColour(), [this](const QColor& colour) {
			this->m_Model->setPrimaryColour(colour);
		}, FloatingPosition::Unspecified);
	});
	connect(secondaryBtn, &ColourButton::triggered, this, [this, secondaryBtn](QAction* action) {
		emit this->m_Model->modalColourSelectRequested(secondaryBtn, this->m_Model->secondaryColour(), [this](const QColor& colour) {
			this->m_Model->setSecondaryColour(colour);
		}, FloatingPosition::Unspecified);
	});

	layout->addWidget(primaryBtn);
	layout->addWidget(secondaryBtn);

	this->setLayout(layout);
}

ColourSelectView::~ColourSelectView() {
}
