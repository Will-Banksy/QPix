#include "AbstractVariantSlider.h"

AbstractVariantSlider::AbstractVariantSlider(QVariant value, QMetaType type, QWidget* parent) : QWidget(parent), m_Value(value), m_Type(type) {
}

AbstractVariantSlider::~AbstractVariantSlider() {
}

void AbstractVariantSlider::setMinimum(QVariant min) {
	assert(min.metaType() == m_Type);
	m_Minimum = min;
}

void AbstractVariantSlider::setMaximum(QVariant max) {
	assert(max.metaType() == m_Type);
	m_Maximum = max;
}

const QVariant& AbstractVariantSlider::value() const {
	return m_Value;
}

const QVariant& AbstractVariantSlider::minimum() const {
	return m_Minimum;
}

const QVariant& AbstractVariantSlider::maximum() const {
	return m_Maximum;
}

void AbstractVariantSlider::setValue(QVariant value) {
	assert(value.metaType() == m_Type);
	if(this->validateValue(value)) {
		emit this->valueChanged(value);
		m_Value = value;
	}
	this->update();
}
