#include "ToolSettingsView.h"
#include "model/AppModel.h"
#include "ui/layouts/FlowLayout.h"
#include <QCheckBox>

// NOTE: Instead of scrapping and regenerating the UI every time the tool changes, it'd probably be better to use a QStackedLayout or something
//       so we don't have to constantly recreate the whole UI - Cheap currently but could get expensive

ToolSettingsView::ToolSettingsView(AppModel* model) : m_Model(model), m_Connections() {
	this->regenUi(model->currentTool());

	this->connect(model, &AppModel::toolChanged, this, [this](AbstractTool* newTool) {
		this->regenUi(newTool);
	});
}

ToolSettingsView::~ToolSettingsView() {
}

void ToolSettingsView::regenUi(AbstractTool* tool) {
	for(QMetaObject::Connection conn : m_Connections) {
		this->disconnect(conn);
	}
	m_Connections.clear();

	FlowLayout* layout = (FlowLayout*)this->layout();
	bool newLayout = false;
	if(layout == nullptr) {
		layout = new FlowLayout();
		newLayout = true;
	} else {
		layout->clear();
	}

	ToolSettings* settings = tool->settings();
	const QVariantMap& settingsMap = settings->getMap();

	qDebug() << "regenUi: mapSize: " << settingsMap.size();

	QMapIterator<QString, QVariant> iter = QMapIterator(settingsMap);
	while(iter.hasNext()) {
		auto entry = iter.next();
		const QString& key = entry.key();
		const QVariant& value = entry.value();

		switch(value.type()) {
			case QVariant::Type::Bool: {
				QCheckBox* checkbox = new QCheckBox(key);
				m_Connections.push_back(this->connect(checkbox, &QCheckBox::checkStateChanged, settings, [checkbox, settings, key](Qt::CheckState state) {
					assert(!checkbox->isTristate());

					if(state == Qt::CheckState::Checked) {
						settings->setValue(key, QVariant(true));
					} else {
						settings->setValue(key, QVariant(false));
					}
				}));
				m_Connections.push_back(this->connect(settings, &ToolSettings::valueChanged, checkbox, [checkbox, settings, key](const QVariant& changedKey, const QVariant& value) {
					assert(value.type() == QVariant::Type::Bool);

					if(changedKey == key) {
						qDebug() << "checkbox setChecked " << value;

						if(value.toBool() == true) {
							checkbox->setCheckState(Qt::CheckState::Checked);
						} else {
							checkbox->setCheckState(Qt::CheckState::Unchecked);
						}
					}
				}));
				layout->addWidget(checkbox);

				break;
			}
		}
	}

	if(newLayout) {
		this->setLayout(layout);
	}
	// this->updateGeometry(); // NOTE: Not sure whether to use this
	this->update();
}