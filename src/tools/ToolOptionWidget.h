#ifndef TOOLOPTIONWIDGET_H
#define TOOLOPTIONWIDGET_H

#include <functional>
#include <QCheckBox>
#include <QComboBox>
#include <QList>
#include <QSpinBox>

typedef std::function<void(QVariant)> OnChangeFunc;

enum ToolOptionType {
	TOT_NONE, // ToolOptionWidget
	TOT_BOOL, // ToolOptionBool
	TOT_MULTI, // ToolOptionMulti
	TOT_INT
};

struct ToolOptionSignals : public QObject {
	Q_OBJECT

public:
	int tool;
	int configIndex;
	ToolOptionSignals(int tool, int configIndex) : tool(tool), configIndex(configIndex) {}

public slots:
	void emitUIUpdate(QVariant newValue) {
		emit uiNeedsUpdate(tool, configIndex, newValue);
	}

signals:
	void uiNeedsUpdate(int tool, int configIndex, QVariant newValue);
};

class ToolOptionWidget {
public:
	ToolOptionWidget() : toolOptionSignals(nullptr) {}
	virtual ~ToolOptionWidget() {
		if(toolOptionSignals) {
			delete toolOptionSignals;
			toolOptionSignals = nullptr;
		}
	}
	const OnChangeFunc onChange;
	ToolOptionSignals* toolOptionSignals;
	virtual ToolOptionType optionType() {
		return TOT_NONE;
	}
	virtual void changeValue(QVariant value) {};
};

class ToolOptionBool : public QCheckBox, public ToolOptionWidget {
public:
	ToolOptionBool(bool initValue, OnChangeFunc func, QString text, QWidget* parent = nullptr);
	virtual ToolOptionType optionType() override {
		return TOT_BOOL;
	}
	virtual void changeValue(QVariant value) override;
};

class ToolOptionMulti : public QComboBox, public ToolOptionWidget {
public:
	ToolOptionMulti(int initValue, OnChangeFunc func, QList<QString> options, QWidget* parent = nullptr);
	virtual ToolOptionType optionType() override {
		return TOT_MULTI;
	}
	virtual void changeValue(QVariant value) override;
};

class ToolOptionInt : public QSpinBox, public ToolOptionWidget {
public:
	ToolOptionInt(int initValue, OnChangeFunc func, int min, int max, QWidget* parent = nullptr);
	virtual ToolOptionType optionType() override {
		return TOT_INT;
	}
	virtual void changeValue(QVariant value) override;
};

#endif // TOOLOPTIONWIDGET_H
