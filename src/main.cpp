#include "pixeleditor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    PixelEditor w;
    w.show();

    return app.exec();
}

