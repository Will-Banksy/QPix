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

	QPoint repositionFloating(const QWidget* self, const QWidget* src, FloatingPosition position) {
		if(position == FloatingPosition::ScreenCentre) {
			QSize half_ws = src->window()->size() / 2;
			return QPoint(
				src->window()->width() / 2 - self->width() / 2,
				src->window()->height() / 2 - self->height() / 2
			);
		}

		const QPoint gap = QPoint(8, 8);

		QPointF srcPos = src->mapTo(src->window(), QPoint(0, 0));
		QSize srcSize = src->size();

		int newX = srcPos.x() + (srcSize.width() / 2) - self->width() / 2;
		int newY = srcPos.y() + (srcSize.height() / 2) - self->height() / 2;

		QPointF newPos = QPoint(newX, newY);

		QPoint toRight = newPos.toPoint() + QPoint(srcSize.width() / 2, 0) + QPoint(self->width() / 2, 0) + QPoint(gap.x(), 0);
		QRect toRightRect = QRect(toRight, self->size());
		QPoint toLeft = newPos.toPoint() - QPoint(srcSize.width() / 2, 0) - QPoint(self->width() / 2, 0) - QPoint(gap.x(), 0);
		QRect toLeftRect = QRect(toLeft, self->size());
		QPoint toBottom = newPos.toPoint() + QPoint(0, srcSize.height() / 2) + QPoint(0, self->height() / 2) + QPoint(0, gap.y());
		QRect toBottomRect = QRect(toBottom, self->size());
		QPoint toTop = newPos.toPoint() - QPoint(0, srcSize.height() / 2) - QPoint(0, self->height() / 2) - QPoint(0, gap.y());
		QRect toTopRect = QRect(toTop, self->size());

		QRect outerRect = QRect(0, 0, self->window()->width(), self->window()->height());

		switch(position) {
			case FloatingPosition::Right: {
				QRect adjusted = utils::adjustedToWithin(outerRect, toRightRect);
				return adjusted.topLeft();
			}
			case FloatingPosition::Left: {
				QRect adjusted = utils::adjustedToWithin(outerRect, toLeftRect);
				return adjusted.topLeft();
			}
			case FloatingPosition::Bottom: {
				QRect adjusted = utils::adjustedToWithin(outerRect, toBottomRect);
				return adjusted.topLeft();
			}
			case FloatingPosition::Top: {
				QRect adjusted = utils::adjustedToWithin(outerRect, toTopRect);
				return adjusted.topLeft();
			}
			case FloatingPosition::Unspecified: {
				int toRightIntersectionArea = utils::area(outerRect.intersected(toRightRect));
				int toLeftIntersectionArea = utils::area(outerRect.intersected(toLeftRect));
				int toBottomIntersectionArea = utils::area(outerRect.intersected(toBottomRect));
				int toTopIntersectionArea = utils::area(outerRect.intersected(toTopRect));

				int minArea = std::max({
					toRightIntersectionArea,
					toLeftIntersectionArea,
					toBottomIntersectionArea,
					toTopIntersectionArea
				});

				QRect rect = QRect();
				if(minArea == toRightIntersectionArea) {
					rect = toRightRect;
				} else if(minArea == toLeftIntersectionArea) {
					rect = toLeftRect;
				} else if(minArea == toBottomIntersectionArea) {
					rect = toBottomRect;
				} else if(minArea == toTopIntersectionArea) {
					rect = toTopRect;
				}

				QRect adjusted = utils::adjustedToWithin(outerRect, rect);

				return adjusted.topLeft();
			}
		}

		assert(false);
	}

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