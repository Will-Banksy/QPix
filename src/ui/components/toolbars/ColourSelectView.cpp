#include "ColourSelectView.h"
#include "ui/widgets/ColourButton.h"
#include "model/AppModel.h"
#include "ui/layouts/FlowLayout.h"

ColourSelectView::ColourSelectView(AppModel* model) : QWidget() {
	// TODO: Layout: A box containing two colour tool buttons (custom widget) diagonally to each other, slightly overlapped

	// TODO: UI for changing the colour. Popup on colour button press, and aseprite-style selector always shown below/alongside buttons for selecting the primary colour

	FlowLayout* layout = new FlowLayout();

	ColourButton* primaryBtn = new ColourButton(model->primaryColour());
	ColourButton* secondaryBtn = new ColourButton(model->secondaryColour());

	connect(model, &AppModel::primaryColourChanged, primaryBtn, &ColourButton::updateColour);
	connect(model, &AppModel::secondaryColourChanged, secondaryBtn, &ColourButton::updateColour);

	layout->addWidget(primaryBtn);
	layout->addWidget(secondaryBtn);

	this->setLayout(layout);
}

ColourSelectView::~ColourSelectView() {
}
