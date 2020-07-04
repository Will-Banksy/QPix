#include "ui/pixeleditor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    PixelEditor mainWindow;
	mainWindow.show();

    return app.exec();
}

