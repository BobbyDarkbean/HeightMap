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
    stream.setFieldAlignment(QTextStream::AlignRight);

    for (std::vector<PeakInfo>::const_iterator itr = t->peaks().begin(); itr != t->peaks().end(); ++itr) {
        stream << "{ "
               << "x: " << qSetFieldWidth(4) << itr->x << qSetFieldWidth(0) << " ; "
               << "y: " << qSetFieldWidth(4) << itr->y << qSetFieldWidth(0) << " ; "
               << "level: " << qSetFieldWidth(6) << itr->height << qSetFieldWidth(0)
               << " }" << endl;
    }
}

Terrain *PeaksTextIOHandler::read(const QString &)
{
    return nullptr;
}


PeaksTextIOHandler::~PeaksTextIOHandler() { }


} // namespace HeightMap
