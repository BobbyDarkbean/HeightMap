#ifndef _Line2dSegment_h_
#define _Line2dSegment_h_


#include "hm_shared.h"


namespace HeightMap {


struct HM_SHARED Line2dSegment
{
    double x1, y1, x2, y2;
    int level;
};


} // namespace HeightMap


#endif // _Line2dSegment_h_
