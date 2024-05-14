// #include "ui/PixelEditor.h"
#include <QApplication>
#include <QStyle>
#include <iostream>
#include <QFile>
#include <QCommonStyle>
// #include "utils/Helper.h"
#include <cmath>
#include "Application.h"
#include "ui/components/AppView.h"
#include "model/AppModel.h"

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

	// return Application::init(argc, argv); // NOTE: Normal QPix

	QApplication app(argc, argv);

	AppModel model = AppModel();
	model.newProject();

	AppView view = AppView(&model);
	view.show();

	int ret = app.exec();

	std::cerr << model.projects()->size() << std::endl;
}