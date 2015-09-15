#include "heightmapapplication.h"
#include "heightmapwindow.h"

int main(int argc, char **argv)
{
    HeightMap::HeightMapApplication app(argc, argv);

    HeightMap::HeightMapWindow w;
    w.showMaximized();

    return app.exec();
}
