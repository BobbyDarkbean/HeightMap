#ifndef _MappingData_h_
#define _MappingData_h_


class QImage;
namespace HeightMap {


class Terrain;
struct MappingData
{
    Terrain *terrain;

    QImage *imgLandscape;
    QImage *imgIsobars;
    QImage *imgHybrid;
};


} // namespace HeightMap


#endif // _MappingData_h_
