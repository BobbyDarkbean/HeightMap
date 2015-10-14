#ifndef _HeightMapIoHandler_h_
#define _HeightMapIoHandler_h_


#include "io_shared.h"


class QString;
namespace HeightMap {


class Terrain;
class HMIO_SHARED HeightMapIOHandler
{
public:
    HeightMapIOHandler();

    virtual void write(const Terrain *t, const QString &filename) = 0;
    virtual void read(Terrain *t, const QString &filename) = 0;

    virtual ~HeightMapIOHandler();
};


} // namespace HeightMap


#endif // _HeightMapIoHandler_h_
