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
#include "Main.h"

// TODO: Add some kind of chunking system to optimise rendering/drawing?

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	app.setStyle("fusion");

	// Load and set palette from json
	QPalette base = app.palette();
	QPalette pal = utils::loadPaletteFrom(":/data/palette.json", base);
	app.setPalette(pal);

	// Load and set style sheet
	QFile qss(":/data/style.qss");
	qss.open(QFile::ReadOnly);
	QString styleSheet = QString::fromUtf8(qss.readAll());
	app.setStyleSheet(styleSheet);

	// TODO: Perhaps load SVG icons at runtime and string replace to colourise them to the correct colour (same as text most likely)

	AppModel model = AppModel();
	model.newProject(DEFAULT_CANVAS_WIDTH, DEFAULT_CANVAS_HEIGHT);
	model.newProject(800, 800);

	AppView view = AppView(&model);
	view.show();

	int ret = app.exec();

    std::cerr << model.projects().size() << std::endl;

    return ret;
}