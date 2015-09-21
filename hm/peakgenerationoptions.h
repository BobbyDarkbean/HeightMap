#ifndef _PeakGenerationOptions_h_
#define _PeakGenerationOptions_h_


#include "hm_shared.h"


namespace HeightMap {


struct HM_SHARED PeakGenerationOptions
{
    unsigned int peakCount;
    int hmWidth, hmHeight, minPeak, maxPeak;
    double baseLvl;
};


} // namespace HeightMap


#endif // _PeakGenerationOptions_h_
