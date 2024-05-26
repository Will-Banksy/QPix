#include "Utils.h"
#include <QJsonDocument>
#include <QFile>
#include <QJsonArray>
#include <iostream>
#include <QJsonObject>
#include <cmath>
#include <QImage>

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

namespace utils { // TODO: Relocate contents to Utils class
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
		QFile qss(":/style_old.qss");
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

	uint32_t* colAt(uint32_t* argbArr, int width, int i, int j) {
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

	QPalette loadPaletteFrom(const QString& src, QPalette& basePalette) {
		// Declare mapping from string to QPalette::ColorRole
		static std::map<QString, QPalette::ColorRole> const table = {
			{ "Window", QPalette::Window },
			{ "WindowText", QPalette::WindowText },
			{ "Base", QPalette::Base },
			{ "AlternateBase", QPalette::AlternateBase },
			{ "ToolTipBase", QPalette::ToolTipBase },
			{ "ToolTipText", QPalette::ToolTipText },
			{ "PlaceholderText", QPalette::PlaceholderText },
			{ "Text", QPalette::Text },
			{ "Button", QPalette::Button },
			{ "ButtonText", QPalette::ButtonText },
			{ "BrightText", QPalette::BrightText },
			{ "Light", QPalette::Light },
			{ "Midlight", QPalette::Midlight },
			{ "Dark", QPalette::Dark },
			{ "Mid", QPalette::Mid },
			{ "Shadow", QPalette::Shadow },
			{ "Highlight", QPalette::Highlight },
			{ "Accent", QPalette::Accent },
			{ "HighlightedText", QPalette::HighlightedText }
		};

		// Open
		QFile file = QFile(src); // TODO: This needs some error handling
		file.open(QIODevice::ReadOnly);
		QByteArray json = file.readAll();

		QPalette newPalette = QPalette(basePalette);

		// Parse
		QJsonDocument doc = QJsonDocument::fromJson(json);

		QJsonObject baseObj = doc.object();
		for(QString key : baseObj.keys()) {
			QString value = baseObj.value(key).toString();

			if(auto it = table.find(key); it != table.end()) {
				QPalette::ColorRole role = it->second;

				QColor col = QColor::fromString(value);

				newPalette.setColor(role, col);
			}
		}

		return newPalette;
	}

	QRect adjustedToWithin(const QRect& outer, const QRect& inner) {
		QRect r = QRect(inner);
		if(r.x() < outer.x()) {
			r.setX(outer.x());
		}
		if(r.y() < outer.y()) {
			r.setY(outer.y());
		}
		if(r.right() > outer.right()) {
			r.setX(outer.right() - r.width());
		}
		if(r.bottom() > outer.bottom()) {
			r.setY(outer.bottom() - r.height());
		}
		return r;
	}

	int area(const QRect& rect) {
		return rect.width() * rect.height();
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

	// a = divisor, n = dividend, See Knuth's definition: https://en.wikipedia.org/wiki/Modulo_operation#In_programming_languages
	int mod(int a, int n) {
		return a - n * floor((float)a / n);
	}

	QList<QPoint> plotLine(int startX, int startY, int endX, int endY) {
		int& x0 = startX;
		int& y0 = startY;
		int& x1 = endX;
		int& y1 = endY;

		int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
		int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
		int err = dx+dy, e2; /* error value e_xy */

		QList<QPoint> points;

		for(;;){  /* loop */
			// Call the function with the () operator
			// 			action(x0, y0, col, settings);
			points.append(QPoint(x0, y0));
			if (x0==x1 && y0==y1) break;
			e2 = 2*err;
			if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
				if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
		}
		return points;
	}

	bool contains(QImage& image, QPoint& point) {
		return (point.x() >= 0 && point.x() < image.width() && point.y() >= 0 && point.y() < image.height());
	}
}