#include "Helper.h"
#include <QJsonDocument>
#include <QFile>
#include <QJsonArray>
#include <iostream>
#include <QJsonObject>
#include <cmath>

// using namespace utils;

/**
 * Uncertain if this actually works - should in theory
 */
// char* addstrs(char* str1, char* str2, int str1len, int str2len) {
// 	char* newarr = (char*)malloc(str1len + str2len);
// 	memcpy((void*)newarr, (void*)str1, str1len);
// 	memcpy((void*)&newarr[str1len], (void*)str2, str2len);
// 	return newarr;
// }

namespace utils {
	float map(float val, float start1, float stop1, float start2, float stop2) {
		return start2 + (stop2 - start2) * ((val - start1) / (stop1 - start1));
	}

	QString getStyleSheet() {
		// Open the JSON file
		QFile json(":/colours.json");
		json.open(QIODevice::ReadOnly);
		QByteArray data = json.readAll();

		// Parse it as JSON
		QJsonDocument doc = QJsonDocument::fromJson(data);

		// Load the QSS file
		QFile qss(":/style.qss");
		qss.open(QFile::ReadOnly);
		QString styleSheet = QString::fromUtf8(qss.readAll());

		// Get the keys and iterate over them
		QJsonObject obj = doc.object();
		for(QString key : obj.keys()) {
			QString val = obj.value(key).toString();

			styleSheet.replace(key, val);
		}

		return styleSheet;
	}

	uint* colAt(uint* argbArr, int width, int i, int j) {
		int index = i + j * width;
		return &argbArr[index];
	}

	QRect expandRound(const QRectF& rect) {
		QPointF p1 = rect.topLeft();
		QPointF p2 = rect.bottomRight();
		int x1 = floor(p1.x());
		int y1 = floor(p1.y());
		int x2 = ceil(p2.x());
		int y2 = ceil(p2.y());
		int w = x2 - x1;
		int h = y2 - y1;
		return QRect(x1, y1, w, h);
	}

	void setBit(quint8& byte, quint8 bitPos, bool val) {
		quint8 mask = 1u << bitPos; // Make a bit mask - the bit at the position we want to change is 1, the rest are 0
		byte &= ~mask; // Sets the bit at bitPos to 0 (if bitPos is 2, does somenumber & 11111011)
		byte |= val << bitPos; // Sets the bit at bitPos to val
	}

	bool getBit(const quint8& byte, quint8 bitPos) {
		quint8 tmp = byte >> bitPos;
		bool val = tmp & 1u;
		return val;
	}
}