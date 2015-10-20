#ifndef _HeightMapBinaryIoHandler_h_
#define _HeightMapBinaryIoHandler_h_


#include "heightmapiohandler.h"
#include "preferences.h"
#include "extrapolationdata.h"


namespace HeightMap {


class HMIO_SHARED HeightMapBinaryIOHandler : public HeightMapIOHandler
{
public:
    HeightMapBinaryIOHandler();

    Preferences preferences() const;
    void setPreferences(const Preferences &);

    ExtrapolationData xData() const;
    void setXData(const ExtrapolationData &);

    void write(const Terrain *t, const QString &filename);
    void read(Terrain *t, const QString &filename);

    ~HeightMapBinaryIOHandler();

private:
    Preferences m_prefs;
    ExtrapolationData m_xdata;
};


} // namespace HeightMap


#endif // _HeightMapBinaryIoHandler_h_
