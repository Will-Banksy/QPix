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
		const QVariantMap& settingsMap = settings->getMap();

		FlowLayout* settingsLayout = new FlowLayout();

		QMapIterator<QString, QVariant> iter = QMapIterator(settingsMap);
		while(iter.hasNext()) {
			auto entry = iter.next();
			const QString& key = entry.key();
			const QVariant& value = entry.value();

			switch(value.type()) {
				case QVariant::Type::Bool: {
					QCheckBox* checkbox = new QCheckBox(key);
					this->connect(checkbox, &QCheckBox::checkStateChanged, settings, [checkbox, settings, key](Qt::CheckState state) {
						assert(!checkbox->isTristate());

						if(state == Qt::CheckState::Checked) {
							settings->setValue(key, QVariant(true));
						} else {
							settings->setValue(key, QVariant(false));
						}
					});
					this->connect(settings, &ToolSettings::valueChanged, checkbox, [checkbox, settings, key](const QVariant& changedKey, const QVariant& value) {
						assert(value.type() == QVariant::Type::Bool);

						if(changedKey == key) {
							qDebug() << "checkbox setChecked " << value;

							if(value.toBool() == true) {
								checkbox->setCheckState(Qt::CheckState::Checked);
							} else {
								checkbox->setCheckState(Qt::CheckState::Unchecked);
							}
						}
					});
					settingsLayout->addWidget(checkbox);

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