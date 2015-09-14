#include <QApplication>
#include "heightmapwindow.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    HeightMap::HeightMapWindow w;
    w.show();

    return app.exec();
}
