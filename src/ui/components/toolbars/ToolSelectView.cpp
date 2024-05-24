#include "ToolSelectView.h"
#include <QVBoxLayout>
#include "model/AppModel.h"
#include <QToolButton>
#include <QActionGroup>
#include <iostream>

ToolSelectView::ToolSelectView(AppModel* model) : QToolBar(), m_ActionGroup(new QActionGroup(this)) {
	for(AbstractTool* tool : model->availableTools()) {
		QAction* selectToolAction = new QAction(QIcon(tool->iconPath()), tool->name());
		selectToolAction->setCheckable(true);
		selectToolAction->setToolTip(tool->description());

		// Connect signals so that each button checked state corresponds with the tool selected in the model
		connect(selectToolAction, &QAction::toggled, [model, tool](bool checked) {
			std::cerr << "selectToolAction toggled" << std::endl;
			if(checked) {
				model->changeTool(tool);
			}
		});
		connect(model, &AppModel::toolChanged, [selectToolAction, tool](AbstractTool* newTool) {
			std::cerr << "toolChanged" << std::endl;
			if(newTool == tool) {
				selectToolAction->setChecked(true);
			}
		});

		// TODO: We want to be able to use floating infos for tooltips here... Which means we need a widget. QToolButton

		if(model->currentTool() == tool) {
			std::cerr << "currentTool is this, setting checked" << std::endl;
			selectToolAction->setChecked(true);
		}

		m_ActionGroup->addAction(selectToolAction);
	}

	m_ActionGroup->setEnabled(true); // TODO: Perhaps only enable when a canvas is available
	m_ActionGroup->setExclusive(true);
	this->addActions(m_ActionGroup->actions());
}

ToolSelectView::~ToolSelectView() {
	m_ActionGroup->deleteLater(); // NOTE: I don't know if ownership of each action is transferred to the action group
}
