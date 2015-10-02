#include "heightmapapplication.h"
#include "heightmaplogic.h"
#include "heightmapwindow.h"


int main(int argc, char **argv)
{
    HeightMap::HeightMapApplication app(argc, argv);
    app.setApplicationName("HeightMap");

    HeightMap::HeightMapWindow w;
    w.setWindowTitle(app.applicationName());
    w.showMaximized();

    app.logic()->newTerrain();
    return app.exec();
}
