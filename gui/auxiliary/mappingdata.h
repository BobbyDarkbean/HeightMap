#ifndef _MappingData_h_
#define _MappingData_h_


class QImage;
namespace HeightMap {


struct MappingData
{
    QImage *imgPeaks;
    QImage *imgLandscape;
    QImage *imgIsobars;
    QImage *imgHybrid;
};


} // namespace HeightMap


#endif // _MappingData_h_
