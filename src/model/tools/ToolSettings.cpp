#include "ToolSettings.h"

// TSVariant

TSVariant::TSVariant() : m_Inner(unit()) {
}

// Copy cons
TSVariant::TSVariant(const TSVariant& other) : m_Inner(other.m_Inner) {
}

// Move cons
TSVariant::TSVariant(TSVariant&& other) : m_Inner(other.m_Inner) {
}

// Copy cons
TSVariant& TSVariant::operator=(const TSVariant& other) {
	if(&other != this) {
		m_Inner = TSVariantInner(other.m_Inner);
	}
	return *this;
}

// Move cons
TSVariant& TSVariant::operator=(TSVariant&& other) {
	if(&other != this) {
		m_Inner = std::move(other.m_Inner);
	}
	return *this;
}

TSVariant TSVariant::newBool(bool value) {
	TSVariantInner inner = TSVariant::consInner<bool>(value);
	return TSVariant(std::move(inner));
}

TSVariant TSVariant::newInRangeU32(TSInRange<u32> value) {
	TSVariantInner inner = TSVariant::consInner<TSInRange<u32>>(value);
	return TSVariant(std::move(inner));
}

TSVariant::InnerType TSVariant::type() const {
	switch(m_Inner.index()) {
		case 1: {
			return InnerType::Bool;
		}
		case 2: {
			return InnerType::TSInRangeU32;
		}
		default: {
			assert(false);
			return *(InnerType*)nullptr;
		}
	}
}

bool TSVariant::isBool() const {
	return this->type() == InnerType::Bool;
}

bool TSVariant::isInRangeU32() const {
	return this->type() == InnerType::TSInRangeU32;
}

bool TSVariant::toBool() const {
	assert(this->isBool());
	return std::get<bool>(m_Inner);
}

TSInRange<u32> TSVariant::toInRangeU32() const {
	assert(this->isInRangeU32());
	return std::get<TSInRange<u32>>(m_Inner);
}

TSVariant::TSVariant(TSVariantInner&& inner) : m_Inner(inner) {
}

// ToolSettings

ToolSettings::ToolSettings(const QMap<QString, ToolSettingInfo>& settingsMap) : m_SettingsMap(settingsMap) {
}

ToolSettings::~ToolSettings() {
}

const Option<ToolSettingInfo> ToolSettings::get(const QString& key) const {
	if(m_SettingsMap.contains(key)) {
		QMap<QString, ToolSettingInfo>::const_iterator valI = m_SettingsMap.find(key);

		return Option<ToolSettingInfo>::newSome(*const_cast<ToolSettingInfo*>(valI.operator->()));
	} else {
		return Option<ToolSettingInfo>::newNone();
	}
}

const QMap<QString, ToolSettingInfo>& ToolSettings::getMap() const {
	return m_SettingsMap;
}

void ToolSettings::setMap(const QMap<QString, ToolSettingInfo>& settingsMap) {
	m_SettingsMap = settingsMap;
}

void ToolSettings::setValue(const QString& key, const TSVariant& value) {
	ToolSettingInfo info = this->get(key).some();
	info.Value = value;
	emit this->valueChanged(key, info);
	m_SettingsMap[key].Value = value;
}
