#include "ToolSelectView.h"
#include <QVBoxLayout>
#include "model/AppModel.h"
#include <QToolButton>
#include <QActionGroup>
#include <iostream>
#include "../floating/HoverInfoEventFilter.h"

ToolSelectView::ToolSelectView(AppModel* model) : QToolBar(), m_ActionGroup(new QActionGroup(this)) {
	this->setFloatable(false);

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
	}

	m_ActionGroup->setEnabled(true); // NOTE: Perhaps only enable when a canvas is available
	m_ActionGroup->setExclusive(true);
	this->addActions(m_ActionGroup->actions());

	// Now loop over all the actions and get the related widgets

	QList<QAction*> actions = m_ActionGroup->actions();
	for(int i = 0; i < model->availableTools().size(); i++) {
		QAction* action = actions.at(i);
		AbstractTool* tool = model->availableTools().at(i);

		QWidget* widget = this->widgetForAction(action);
		widget->installEventFilter(new HoverInfoEventFilter(
			model,
			widget,
			tool->name(),
			tool->description()
		));
	}
}

ToolSelectView::~ToolSelectView() {
	m_ActionGroup->deleteLater(); // NOTE: I don't know if ownership of each action is transferred to the action group
}
