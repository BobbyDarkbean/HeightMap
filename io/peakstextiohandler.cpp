#include <QFile>
#include <QTextStream>
#include "terrain.h"

#include "peakstextiohandler.h"


namespace HeightMap {


PeaksTextIOHandler::PeaksTextIOHandler()
    : HeightMapIOHandler() { }


void PeaksTextIOHandler::write(const Terrain *t, const QString &filename)
{
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        return;
    }

    QTextStream stream(&f);
    stream.setRealNumberPrecision(2);
    stream.setRealNumberNotation(QTextStream::FixedNotation);

    for (std::vector<PeakInfo>::const_iterator itr = t->peaks().begin(); itr != t->peaks().end(); ++itr) {

    }
}

void PeaksTextIOHandler::read(Terrain *, const QString &)
{

}


PeaksTextIOHandler::~PeaksTextIOHandler() { }


} // namespace HeightMap
