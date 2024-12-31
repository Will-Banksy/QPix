#include "ToolSettingsView.h"
#include "model/AppModel.h"
#include "ui/layouts/FlowLayout.h"
#include <QCheckBox>
#include <QStackedLayout>

ToolSettingsView::ToolSettingsView(AppModel* model) : m_Model(model) {
	const QList<AbstractTool*>& tools = model->availableTools();

	m_Layout = new QStackedLayout();

	m_Layout->addWidget(new QWidget());

	int index = 1;

	for(AbstractTool* tool : tools) {
		ToolSettings* settings = tool->settings();
		const QMap<QString, TSVariant>& settingsMap = settings->getMap();

		FlowLayout* settingsLayout = new FlowLayout();

		QMapIterator<QString, TSVariant> iter = QMapIterator(settingsMap);
		while(iter.hasNext()) {
			auto entry = iter.next();
			const QString& key = entry.key();
			const TSVariant& value = entry.value();

			switch(value.type()) {
				case TSVariant::InnerType::Bool: {
					QCheckBox* checkbox = new QCheckBox(key);

					this->connect(checkbox, &QCheckBox::checkStateChanged, settings, [checkbox, settings, key](Qt::CheckState state) {
						assert(!checkbox->isTristate());

						if(state == Qt::CheckState::Checked) {
							settings->setValue(key, TSVariant::newBool(true));
						} else {
							settings->setValue(key, TSVariant::newBool(false));
						}
					});
					this->connect(settings, &ToolSettings::valueChanged, checkbox, [checkbox, settings, key](const QString& changedKey, const TSVariant& value) {
						assert(value.type() == TSVariant::InnerType::Bool);

						if(changedKey == key) {
							if(value.toBool() == true) {
								checkbox->setCheckState(Qt::CheckState::Checked);
							} else {
								checkbox->setCheckState(Qt::CheckState::Unchecked);
							}
						}
					});
					// Emit settings changed to update UI
					emit settings->valueChanged(key, settings->get(key).some());
					settingsLayout->addWidget(checkbox);

					break;
				}
				case TSVariant::InnerType::TSInRangeU32: {
					// TODO
					assert(false);
					break;
				}
			}
		}

		QWidget* widget = new QWidget();
		widget->setLayout(settingsLayout);
		m_Layout->addWidget(widget);

		m_IndexMap.insert(tool, index);
		index += 1;
	}

	this->setLayout(m_Layout);

	this->switchUi(model->currentTool());

	this->connect(model, &AppModel::toolChanged, this, [this](AbstractTool* newTool) {
		this->switchUi(newTool);
	});
}

ToolSettingsView::~ToolSettingsView() {
}

void ToolSettingsView::switchUi(AbstractTool* tool) {
	m_Layout->setCurrentIndex(m_IndexMap.value(tool, 0));
}