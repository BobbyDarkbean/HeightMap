#include <QFile>
#include <QDataStream>
#include "terrain.h"

#include "heightmapbinaryiohandler.h"


namespace HeightMap {


HeightMapBinaryIOHandler::HeightMapBinaryIOHandler()
    : HeightMapIOHandler() { }


void HeightMapBinaryIOHandler::write(const Terrain *t, const QString &filename)
{
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly)) {
        return;
    }

    QDataStream stream(&f);
    stream.setVersion(QDataStream::Qt_5_0);

}

void HeightMapBinaryIOHandler::read(Terrain *t, const QString &filename)
{
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) {
        return;
    }

    QDataStream stream(&f);
    stream.setVersion(QDataStream::Qt_5_0);

}


HeightMapBinaryIOHandler::~HeightMapBinaryIOHandler() { }


} // namespace HeightMap
