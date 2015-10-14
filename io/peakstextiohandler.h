#ifndef _PeaksTextIoHandler_h_
#define _PeaksTextIoHandler_h_


#include "heightmapiohandler.h"


namespace HeightMap {


class HMIO_SHARED PeaksTextIOHandler : public HeightMapIOHandler
{
public:
    PeaksTextIOHandler();

    void write(const Terrain *t, const QString &filename);
    void read(Terrain *t, const QString &filename);

    ~PeaksTextIOHandler();
};


} // namespace HeightMap


#endif // _PeaksTextIoHandler_h_
