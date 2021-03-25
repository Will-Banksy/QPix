#ifndef ICONS_H
#define ICONS_H

#include <QImage>
#include <QHash>

class Icons {
public:
	static QHash<QString, QImage*> iconsMap;
	static const QList<QString> iconStrs;
	static void setIconsColour(QColor colour);
	static void loadIcons(QColor colour);
};

#endif
