#ifndef _PeakGenerationOptions_h_
#define _PeakGenerationOptions_h_


#include "hm_shared.h"


namespace HeightMap {


struct HM_SHARED PeakGenerationOptions
{
    unsigned int peakCount, hmWidth, hmHeight;
    int minPeak, maxPeak;
};


} // namespace HeightMap


#endif // _PeakGenerationOptions_h_