#ifndef _PeakInfo_h_
#define _PeakInfo_h_


#include "hm_shared.h"


namespace HeightMap {


struct HM_SHARED PeakInfo
{
    int x, y;
    double height;
};


inline bool peakHeightLessThan(const PeakInfo &a, const PeakInfo &b)
{ return a.height < b.height; }


inline bool peakCoordsLessThan(const PeakInfo &a, const PeakInfo &b)
{ return (a.x < b.x) ? true : (a.x > b.x) ? false : (a.y < b.y); }


} // namespace HeightMap


#endif // _PeakInfo_h_
