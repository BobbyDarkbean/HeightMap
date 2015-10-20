#include <QFile>
#include <QDataStream>
#include "terrain.h"

#include "heightmapbinaryiohandler.h"


namespace HeightMap {


HeightMapBinaryIOHandler::HeightMapBinaryIOHandler()
    : HeightMapIOHandler() { }


Preferences HeightMapBinaryIOHandler::preferences() const
{ return m_prefs; }

void HeightMapBinaryIOHandler::setPreferences(const Preferences &prefs)
{ m_prefs = prefs; }

ExtrapolationData HeightMapBinaryIOHandler::xData() const
{ return m_xdata; }

void HeightMapBinaryIOHandler::setXData(const ExtrapolationData &data)
{ m_xdata = data; }


void HeightMapBinaryIOHandler::write(const Terrain *t, const QString &filename)
{
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly)) {
        return;
    }

    QDataStream stream(&f);
    stream.setVersion(QDataStream::Qt_5_0);

    stream << m_prefs << m_xdata;
    for (std::vector<PeakInfo>::const_iterator i = t->peaks().begin(); i != t->peaks().end(); ++i) {
        stream << static_cast<quint32>(i->x)
               << static_cast<quint32>(i->y)
               << i->height;    // TODO: cast to independent format
    }
}

Terrain *HeightMapBinaryIOHandler::read(const QString &filename)
{
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) {
        return nullptr;
    }

    QDataStream stream(&f);
    stream.setVersion(QDataStream::Qt_5_0);

    stream >> m_prefs >> m_xdata;
    std::vector<PeakInfo> peaks(m_prefs.peakCount());
    for (std::vector<PeakInfo>::iterator i = peaks.begin(); i != peaks.end(); ++i) {
        quint32 x, y;
        double height;

        stream >> x >> y >> height;
        i->x = static_cast<int>(x);
        i->y = static_cast<int>(y);
        i->height = height;     // TODO: cast from independent format
    }

    Terrain *t = new Terrain(m_prefs.landscapeWidth(), m_prefs.landscapeHeight());
    t->setPeaks(peaks);
    return t;
}


HeightMapBinaryIOHandler::~HeightMapBinaryIOHandler() { }


} // namespace HeightMap
