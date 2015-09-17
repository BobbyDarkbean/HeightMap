#ifndef _MappingData_h_
#define _MappingData_h_


#include <vector>
#include <list>
#include "peakinfo.h"
#include "line2dsegment.h"


class QImage;
namespace HeightMap {


struct PeakGenerationOptions;
class Landscape;
struct MappingData
{
    PeakGenerationOptions *genOptions;
    std::vector<PeakInfo> *peaks;
    std::vector<int> *levels;
    std::list<Line2dSegment> *contours;
    Landscape *landscape;

    QImage *imgLandscape;
    QImage *imgIsobars;
    QImage *imgHybrid;
};


} // namespace HeightMap


#endif // _MappingData_h_
