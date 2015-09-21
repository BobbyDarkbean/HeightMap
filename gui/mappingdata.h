#ifndef _MappingData_h_
#define _MappingData_h_


#include <vector>


class QImage;
namespace HeightMap {


struct PeakGenerationOptions;
class Terrain;
struct MappingData
{
    PeakGenerationOptions *genOptions;
    std::vector<int> *levels;
    Terrain *terrain;

    QImage *imgLandscape;
    QImage *imgIsobars;
    QImage *imgHybrid;

    int imageFactor;
};


} // namespace HeightMap


#endif // _MappingData_h_
