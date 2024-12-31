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

ToolSettings::ToolSettings(const QMap<QString, TSVariant>& settingsMap) : m_SettingsMap(settingsMap) {
}

ToolSettings::~ToolSettings() {
}

const Option<TSVariant> ToolSettings::get(const QString& key) const {
	if(m_SettingsMap.contains(key)) {
		QMap<QString, TSVariant>::const_iterator valI = m_SettingsMap.find(key);

		return Option<TSVariant>::newSome(*const_cast<TSVariant*>(valI.operator->()));
	} else {
		return Option<TSVariant>::newNone();
	}
}

const QMap<QString, TSVariant>& ToolSettings::getMap() const {
	return m_SettingsMap;
}

void ToolSettings::setMap(const QMap<QString, TSVariant>& settingsMap) {
	m_SettingsMap = settingsMap;
}

void ToolSettings::setValue(const QString& key, const TSVariant& value) {
	emit this->valueChanged(key, value);
	m_SettingsMap[key] = value;
}
