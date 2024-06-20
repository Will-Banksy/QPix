#include "ToolSelectView.h"
#include <QVBoxLayout>
#include "model/AppModel.h"
#include <QToolButton>
#include <QActionGroup>
#include <iostream>
#include "../floating/tooltip/HoverInfoEventFilter.h"
#include "../../layouts/FlowLayout.h"

ToolSelectView::ToolSelectView(AppModel* model) : QWidget(), m_ActionGroup(new QActionGroup(this)) {
	// The action group means that the toolbuttons are exclusive, since I set an action for each that is added to the action group
	m_ActionGroup->setEnabled(true);
	m_ActionGroup->setExclusive(true);

	FlowLayout* layout = new FlowLayout();

	for(AbstractTool* tool : model->availableTools()) {
		QAction* selectToolAction = new QAction(QIcon(tool->iconPath()), tool->name());
		selectToolAction->setCheckable(true);

		// Connect signals so that each button checked state corresponds with the tool selected in the model
		connect(selectToolAction, &QAction::toggled, this, [model, tool](bool checked) {
			if(checked) {
				model->changeTool(tool);
			}
		});
		connect(model, &AppModel::toolChanged, this, [selectToolAction, tool](AbstractTool* newTool) {
			if(newTool == tool) {
				selectToolAction->setChecked(true);
			}
		});

		if(model->currentTool() == tool) {
			selectToolAction->setChecked(true);
		}

		m_ActionGroup->addAction(selectToolAction);

		QToolButton* btn = new QToolButton();
		btn->setDefaultAction(selectToolAction);
		btn->setIconSize(QSize(24, 24)); // TODO: How do we determine this?
		btn->setMinimumSize(32, 32);
		btn->setMaximumSize(32, 32);
		btn->installEventFilter(new HoverInfoEventFilter(
			model,
			btn,
			tool->name(),
			tool->description()
		));

		layout->addWidget(btn);
	}

	this->setLayout(layout);
}

ToolSelectView::~ToolSelectView() {
	m_ActionGroup->deleteLater(); // NOTE: I don't know if ownership of each action is transferred to the action group
}
