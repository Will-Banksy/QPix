#include "ToolOptionWidget.h"

ToolOptionBool::ToolOptionBool(OnChangeFunc func, QString text, QWidget* parent) : QCheckBox(text, parent), ToolOptionWidget() {
	QObject::connect(this, QOverload<int>::of(&QCheckBox::stateChanged), [func](int newValue) {
		func(newValue);
	});
}

ToolOptionMulti::ToolOptionMulti(OnChangeFunc func, QList<QString> options, QWidget* parent) : QComboBox(), ToolOptionWidget() {
	for(QString& str : options) {
		addItem(str);
	}
	QObject::connect(this, QOverload<int>::of(&QComboBox::currentIndexChanged), [func](int newValue) {
		func(newValue);
	});
}
