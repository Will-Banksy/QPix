#include "Helper.h"
#include <QJsonDocument>
#include <QFile>
#include <QJsonArray>
#include <iostream>
#include <QJsonObject>

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

float utils::map(float val, float start1, float stop1, float start2, float stop2) {
	return start2 + (stop2 - start2) * ((val - start1) / (stop1 - start1));
}

QString utils::getStyleSheet() {
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

	std::cout << "After: " << styleSheet.toStdString() << std::endl;

	// Get the keys and iterate over them
	QJsonObject obj = doc.object();
	for(QString key : obj.keys()) {
		QString val = obj.value(key).toString();

		styleSheet.replace(key, val);
	}

	std::cout << "After: " << styleSheet.toStdString() << std::endl;

	return styleSheet;
}