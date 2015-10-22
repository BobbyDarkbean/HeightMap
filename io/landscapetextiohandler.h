#ifndef _LandscapeTextIoHandler_h_
#define _LandscapeTextIoHandler_h_


#include "heightmapiohandler.h"


namespace HeightMap {


class HMIO_SHARED LandscapeTextIOHandler : public HeightMapIOHandler
{
public:
    LandscapeTextIOHandler();

    void write(const Terrain *t, const QString &filename);
    Terrain *read(const QString &filename);

    ~LandscapeTextIOHandler();
};


} // namespace HeightMap


#endif // _LandscapeTextIoHandler_h_
