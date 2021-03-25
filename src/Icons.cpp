#include "Icons.h"

QHash<QString, QImage*> Icons::iconsMap;
const QList<QString> Icons::iconStrs = { ":/pencil.png", ":/eraser.png", ":/fill.png", ":/line.png", ":/move.png" };

void Icons::setIconsColour(QColor colour) {
	Q_FOREACH(QImage* image, iconsMap) {
		if(image->format() != QImage::Format_ARGB32) {
			continue;
		}
		quint8* bytes = (quint8*)image->bits();
		for(quint32 i = 0; i < image->width() * image->height() * 4; i++) {
			int argbIndex = i % 4;
			if(argbIndex == 0) {
				bytes[i] = colour.red() & 0xff;
			} else if(argbIndex == 1) {
				bytes[i] = colour.green() & 0xff;
			} else if(argbIndex == 2) {
				bytes[i] = colour.blue() & 0xff;
			}
		}
	}
}

void Icons::loadIcons(QColor colour) {
	for(const QString& str : iconStrs) {
		iconsMap[str] = new QImage(str);
	}
	setIconsColour(colour);
}