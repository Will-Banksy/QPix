#include "ToolSettings.h"

ToolSettings::ToolSettings(const QMap<QString, QVariant>& settingsMap) : m_SettingsMap(settingsMap) {
}

ToolSettings::~ToolSettings() {
}

const Nullable<QVariant> ToolSettings::get(const QString& key) const {
	if(m_SettingsMap.contains(key)) {
		QVariantMap::const_iterator valI = m_SettingsMap.find(key);

		return Nullable<QVariant>(const_cast<QVariant*>(valI.operator->()));
	} else {
		return Nullable<QVariant>(nullptr);
	}
}

const QMap<QString, QVariant>& ToolSettings::getMap() const {
	return m_SettingsMap;
}

void ToolSettings::setMap(const QMap<QString, QVariant>& settingsMap) {
	m_SettingsMap = settingsMap;
}

void ToolSettings::setValue(const QString& key, const QVariant& value) {
	emit this->valueChanged(key, value);
	m_SettingsMap[key] = value;
	qDebug() << "tool settings setValue " << value;
}
