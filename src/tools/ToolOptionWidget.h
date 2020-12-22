#ifndef TOOLOPTIONWIDGET_H
#define TOOLOPTIONWIDGET_H

#include <functional>
#include <QCheckBox>
#include <QComboBox>
#include <QList>

typedef std::function<void(QVariant)> OnChangeFunc;

enum ToolOptionType {
	TOT_NONE, // ToolOptionWidget
	TOT_BOOL, // ToolOptionBool
	TOT_MULTI // ToolOptionMulti
};

class ToolOptionWidget {
public:
	virtual ~ToolOptionWidget() = default;
	const OnChangeFunc onChange;
	virtual ToolOptionType optionType() {
		return TOT_NONE;
	}
	virtual void changeValue(QVariant value) {}
};

class ToolOptionBool : public QCheckBox, public ToolOptionWidget {
public:
	virtual ~ToolOptionBool() override = default;
	ToolOptionBool(OnChangeFunc func, QString text, QWidget* parent = nullptr);
	virtual ToolOptionType optionType() override {
		return TOT_BOOL;
	}
	virtual void changeValue(QVariant value) override { setChecked(value.toBool()); }
};

class ToolOptionMulti : public QComboBox, public ToolOptionWidget {
public:
	virtual ~ToolOptionMulti() override = default;
	ToolOptionMulti(OnChangeFunc func, QList<QString> options, QWidget* parent = nullptr);
	virtual ToolOptionType optionType() override {
		return TOT_MULTI;
	}
	virtual void changeValue(QVariant value) override { setCurrentIndex(value.toInt()); }
};

#endif // TOOLOPTIONWIDGET_H
