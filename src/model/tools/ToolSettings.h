#ifndef TOOLSETTINGS_H
#define TOOLSETTINGS_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include "utils/Nullable.h"

class ToolSettings : public QObject {
	Q_OBJECT

public:
	explicit ToolSettings(const QMap<QString, QVariant>& settingsMap);
	~ToolSettings() override;

	const Nullable<QVariant> get(const QString& key) const;
	const QMap<QString, QVariant>& getMap() const;
	void setMap(const QMap<QString, QVariant>& settingsMap);

public slots:
	void setValue(const QString& key, const QVariant& value);

signals:
	void valueChanged(const QString& key, const QVariant& value);

private:
	QMap<QString, QVariant> m_SettingsMap;
};

#endif // TOOLSETTINGS_H