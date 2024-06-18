// #include "ui/PixelEditor.h"
#include <QApplication>
#include <QStyle>
#include <iostream>
#include <QFile>
#include <QCommonStyle>
#include <cmath>
#include "ui/components/AppView.h"
#include "model/AppModel.h"
#include "model/ProjectModel.h"
#include <QMap>
#include <QPalette>
#include "utils/Utils.h"

int main(int argc, char *argv[]) {
// 	uchar byte = 0b00000100;
// 	utils::setBit(byte, 2, 0);
// 	utils::setBit(byte, 7, 1);
// 	byte = ~byte;
// 	std::cout << utils::getBit(byte, 7) << std::endl;
// 	std::cout << (ulong)byte << std::endl;

// 	QApplication app(argc, argv);

	// 	QStyle* style = app.style();
	// 	std::cout << style->objectName().toStdString() << std::endl;

// 	app.setStyle(new QCommonStyle()); // Basing my QSS-made style off the cross-platform common style so less 'visual artifacts' (aka things not bloody working like they should) appear

	// Get the stylesheet
// 	QString styleSheet = utils::getStyleSheet(); // Maybe make so it can load other style sheets, if the user wants

// 	app.setStyleSheet(styleSheet);

// 	PixelEditor mainWindow;
// 	mainWindow.show();

// 	return app.exec();

	// return Application::init(argc, argv); // NOTE: Normal QPix - Probably broken though.

	QApplication app(argc, argv);

	app.setStyle("fusion");

	// Load palette from json
	QPalette base = app.palette();
	QPalette pal = utils::loadPaletteFrom(":/data/palette.json", base);
	app.setPalette(pal);

	QFile qss(":/data/style.qss");
	qss.open(QFile::ReadOnly);
	QString styleSheet = QString::fromUtf8(qss.readAll());
	app.setStyleSheet(styleSheet);

	AppModel model = AppModel();
	model.newProject(DEFAULT_CANVAS_WIDTH, DEFAULT_CANVAS_HEIGHT);
	model.newProject(800, 800);

	AppView view = AppView(&model);
	view.show();

	int ret = app.exec();

    std::cerr << model.projects().size() << std::endl;

    return ret;
}