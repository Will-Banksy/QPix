#ifndef TOOLSETTINGS_H
#define TOOLSETTINGS_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include "Main.h"
#include "utils/Option.h"

template<typename T>
struct TSInRange {
	T Start;
	T End;
	T Value;

	TSInRange(const TSInRange& other) = default;

	TSInRange withValue(T newValue) const {
		return TSInRange {
			.Start = Start,
			.End = End,
			.Value = newValue
		};
	}
};

class TSVariant {
private:
	typedef std::variant<unit, bool, TSInRange<u32>> TSVariantInner;

public:
	enum class InnerType {
		Bool,
		TSInRangeU32
	};

	TSVariant();

	// Copy cons
	TSVariant(const TSVariant& other);

	// Move cons
	TSVariant(TSVariant&& other);

	// Copy cons
	TSVariant& operator=(const TSVariant& other);

	// Move cons
	TSVariant& operator=(TSVariant&& other);

	static TSVariant newBool(bool value);
	static TSVariant newInRangeU32(TSInRange<u32> value);

	InnerType type() const;
	bool isBool() const;
	bool isInRangeU32() const;

	bool toBool() const;
	TSInRange<u32> toInRangeU32() const;

private:
	TSVariantInner m_Inner;

	explicit TSVariant(TSVariantInner&& inner);

	template<typename I, typename... Args>
	static TSVariantInner consInner(Args&&... args) {
		TSVariantInner inner = TSVariantInner();
		inner.template emplace<I>(std::forward<Args>(args)...);
		return inner;
	}
};

class ToolSettings : public QObject {
	Q_OBJECT

public:
	explicit ToolSettings(const QMap<QString, TSVariant>& settingsMap);
	~ToolSettings() override;

	const Option<TSVariant> get(const QString& key) const;
	const QMap<QString, TSVariant>& getMap() const;
	void setMap(const QMap<QString, TSVariant>& settingsMap);

public slots:
	void setValue(const QString& key, const TSVariant& value);

signals:
	void valueChanged(const QString& key, const TSVariant& value);

private:
	QMap<QString, TSVariant> m_SettingsMap;
};

#endif // TOOLSETTINGS_H