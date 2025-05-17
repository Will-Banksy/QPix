#include "ToolSettingsView.h"
#include "model/AppModel.h"
#include "ui/layouts/FlowLayout.h"
#include <QCheckBox>
#include <QStackedLayout>
#include <QSpinBox>
#include <QLabel>
#include "ui/components/floating/tooltip/HoverInfoEventFilter.h"

ToolSettingsView::ToolSettingsView(AppModel* model) : m_Model(model) {
	const QList<AbstractTool*>& tools = model->availableTools();

	m_Layout = new QStackedLayout();

	m_Layout->addWidget(new QWidget());

	int index = 1;

	for(AbstractTool* tool : tools) {
		ToolSettings* settings = tool->settings();
		const QMap<QString, ToolSettingInfo>& settingsMap = settings->getMap();

		FlowLayout* settingsLayout = new FlowLayout();

		QMapIterator<QString, ToolSettingInfo> iter = QMapIterator(settingsMap);
		while(iter.hasNext()) {
			auto entry = iter.next();
			const QString& key = entry.key();
			const ToolSettingInfo& info = entry.value();

			QHBoxLayout* itemLayout = new QHBoxLayout();
			itemLayout->setSpacing(8);
			itemLayout->setContentsMargins(0, 0, 0, 0);

			switch(info.Value.type()) {
				case TSVariant::InnerType::Bool: {
					QCheckBox* checkbox = new QCheckBox(info.Title);

					this->connect(checkbox, &QCheckBox::checkStateChanged, settings, [checkbox, settings, key](Qt::CheckState state) {
						assert(!checkbox->isTristate());

						if(state == Qt::CheckState::Checked) {
							settings->setValue(key, TSVariant::newBool(true));
						} else {
							settings->setValue(key, TSVariant::newBool(false));
						}
					});
					this->connect(settings, &ToolSettings::valueChanged, checkbox, [checkbox, settings, key](const QString& changedKey, const ToolSettingInfo& newInfo) {
						assert(newInfo.Value.type() == TSVariant::InnerType::Bool);

						if(changedKey == key) {
							if(newInfo.Value.toBool() == true) {
								checkbox->setCheckState(Qt::CheckState::Checked);
							} else {
								checkbox->setCheckState(Qt::CheckState::Unchecked);
							}
						}
					});
					// Emit settings changed to update UI
					emit settings->valueChanged(key, settings->get(key).some());
					itemLayout->addWidget(checkbox);

					break;
				}
				case TSVariant::InnerType::TSInRangeU32: {
					QSpinBox* spinbox = new QSpinBox();
					spinbox->setFixedWidth(60);

					TSVariant value = info.Value;
					this->connect(spinbox, &QSpinBox::valueChanged, settings, [spinbox, value, settings, key](int newValue) {
						settings->setValue(key, TSVariant::newInRangeU32(value.toInRangeU32().withValue(newValue)));
					});
					this->connect(settings, &ToolSettings::valueChanged, spinbox, [spinbox, settings, key](const QString& changedKey, const ToolSettingInfo& newInfo) {
						assert(newInfo.Value.type() == TSVariant::InnerType::TSInRangeU32);

						if(changedKey == key) {
							auto val = newInfo.Value.toInRangeU32();
							spinbox->setMinimum(val.Start);
							spinbox->setMaximum(val.End);
							spinbox->setValue(val.Value);
						}
					});
					emit settings->valueChanged(key, settings->get(key).some());

					QLabel* spinboxLabel = new QLabel(info.Title + ":");

					itemLayout->addWidget(spinboxLabel);
					itemLayout->addWidget(spinbox);

					break;
				}
			}

			QWidget* itemWidget = new QWidget();
			itemWidget->setLayout(itemLayout);
			itemWidget->installEventFilter(new HoverInfoEventFilter(
				model,
				itemWidget,
				info.Title,
				info.Description
			));

			settingsLayout->addWidget(itemWidget);
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