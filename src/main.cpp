#include "ui/PixelEditor.h"
#include <QApplication>
#include <QStyle>
#include <iostream>
#include <QFile>
#include <QCommonStyle>
#include "utils/Helper.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

// 	QStyle* style = app.style();
// 	std::cout << style->objectName().toStdString() << std::endl;

	app.setStyle(new QCommonStyle()); // Basing my QSS-made style off the cross-platform common style so less 'visual artifacts' (aka things not bloody working like they should) appear

	// Get the stylesheet
	QString styleSheet = utils::getStyleSheet();

	app.setStyleSheet(styleSheet);

    PixelEditor mainWindow;
	mainWindow.show();

    return app.exec();
}