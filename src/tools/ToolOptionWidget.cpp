#include "ToolOptionWidget.h"
#include <iostream>

ToolOptionBool::ToolOptionBool(bool initValue, OnChangeFunc func, QString text, QWidget* parent) : QCheckBox(text, parent), ToolOptionWidget() {
	connect(this, QOverload<int>::of(&QCheckBox::stateChanged), [func, this](int newValue) {
		func(newValue);
		if(toolOptionSignals) {
			toolOptionSignals->emitUIUpdate(newValue);
		}
	});
	setChecked(initValue);
}

void ToolOptionBool::changeValue(QVariant value) {
	setChecked(value.toBool());
}

ToolOptionMulti::ToolOptionMulti(int initValue, OnChangeFunc func, QList<QString> options, QWidget* parent) : QComboBox(), ToolOptionWidget() {
	for(QString& str : options) {
		addItem(str);
	}
	connect(this, QOverload<int>::of(&QComboBox::currentIndexChanged), [func, this](int newValue) {
		func(newValue);
		if(toolOptionSignals) {
			toolOptionSignals->emitUIUpdate(newValue);
		}
	});
	setCurrentIndex(initValue);
}

void ToolOptionMulti::changeValue(QVariant value) {
	ToolOptionWidget::changeValue(value);
	setCurrentIndex(value.toInt());
}

ToolOptionInt::ToolOptionInt(int initValue, OnChangeFunc func, int min, int max, QWidget* parent) : QSpinBox(), ToolOptionWidget() {
	setMinimum(min);
	setMaximum(max);
	connect(this, QOverload<int>::of(&QSpinBox::valueChanged), [func, this](int newValue) {
		func(newValue);
		if(toolOptionSignals) {
			toolOptionSignals->emitUIUpdate(newValue);
		}
	});
	setValue(initValue);
	setPrefix("Tolerance: ");
}

void ToolOptionInt::changeValue(QVariant value) {
	ToolOptionWidget::changeValue(value);
	setValue(value.toInt());
}
