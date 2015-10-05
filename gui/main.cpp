#include "heightmapapplication.h"
#include "heightmaplogic.h"
#include "heightmapwindow.h"


int main(int argc, char **argv)
{
    HeightMap::HeightMapApplication app(argc, argv);
    app.setApplicationName("HeightMap");

    HeightMap::HeightMapLogic *logic = app.logic();

    HeightMap::HeightMapWindow w;
    w.setWindowTitle(app.applicationName());
    w.init(logic);
    w.showMaximized();

    logic->newTerrain();
    return app.exec();
}
