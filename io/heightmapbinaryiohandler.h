#ifndef _HeightMapBinaryIoHandler_h_
#define _HeightMapBinaryIoHandler_h_


#include "heightmapiohandler.h"


namespace HeightMap {


class HeightMapBinaryIOHandler : public HeightMapIOHandler
{
public:
    HeightMapBinaryIOHandler();

    void write(const Terrain *t, const QString &filename);
    void read(Terrain *t, const QString &filename);

    ~HeightMapBinaryIOHandler();
};


} // namespace HeightMap


#endif // _HeightMapBinaryIoHandler_h_
