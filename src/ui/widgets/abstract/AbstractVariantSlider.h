#ifndef ABSTRACTVARIANTSLIDER_H
#define ABSTRACTVARIANTSLIDER_H

#include <QWidget>

// TODO: Can I put any more stuff in this class? If not, should it really be an abstract slider class? Perhaps just something like "AbstractVariantEditor" would be more appropriate

class AbstractVariantSlider : public QWidget {
	Q_OBJECT

public:
	explicit AbstractVariantSlider(QVariant value, QMetaType type, QWidget* parent = nullptr);
	~AbstractVariantSlider() override;

	void setMinimum(QVariant min);
	void setMaximum(QVariant max);

	const QVariant& value() const;
	const QVariant& minimum() const;
	const QVariant& maximum() const;

public slots:
	void setValue(QVariant value);

signals:
	void valueChanged(const QVariant& newValue);

protected:
	/// Should be overridden to validate that the new value is valid (this class does already assert that the provided value has the provided QMetaType),
	/// and should also clamp the value to between the minimum and maximum if applicable
	virtual bool validateValue(QVariant& value) = 0;

private:
	QVariant m_Value;
	QMetaType m_Type;
	QVariant m_Minimum;
	QVariant m_Maximum;
};

#endif // ABSTRACTVARIANTSLIDER_H