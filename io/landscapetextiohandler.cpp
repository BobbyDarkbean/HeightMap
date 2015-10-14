#include <QFile>
#include <QTextStream>
#include "terrain.h"
#include "landscape.h"

#include "landscapetextiohandler.h"


namespace HeightMap {


LandscapeTextIOHandler::LandscapeTextIOHandler()
    : HeightMapIOHandler() { }


void LandscapeTextIOHandler::write(const Terrain *t, const QString &filename)
{
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        return;
    }

    QTextStream stream(&f);
    stream.setRealNumberPrecision(4);
    stream.setRealNumberNotation(QTextStream::FixedNotation);

    // transposed matrix
    const double *const d = t->landscape().matrix();
    int w = t->landscape().width();
    int h = t->landscape().height();
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {

        }
    }
}

void LandscapeTextIOHandler::read(Terrain *, const QString &)
{

}


LandscapeTextIOHandler::~LandscapeTextIOHandler() { }


} // namespace HeightMap
