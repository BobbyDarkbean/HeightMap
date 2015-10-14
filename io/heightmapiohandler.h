#ifndef _HeightMapIoHandler_h_
#define _HeightMapIoHandler_h_


class QString;
namespace HeightMap {


class Terrain;
class HeightMapIOHandler
{
public:
    HeightMapIOHandler();

    virtual void write(const Terrain *t, const QString &filename) = 0;
    virtual void read(Terrain *t, const QString &filename) = 0;

    virtual ~HeightMapIOHandler();
};


} // namespace HeightMap


#endif // _HeightMapIoHandler_h_
