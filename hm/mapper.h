#ifndef _Mapper_h_
#define _Mapper_h_


#include <vector>
#include <list>
#include "hm_shared.h"
#include "event.h"
#include "peakinfo.h"
#include "line2dsegment.h"
#include "peakgenerationoptions.h"


export_event(HM_TEMPLATE, HM_SHARED, HeightMap::PeakInfo);
export_event(HM_TEMPLATE, HM_SHARED, int);


namespace HeightMap {


class Landscape;
class HM_SHARED Mapper
{
    enum E {
        peakGenerated,
        peakExtrapolated,
        contouringAt
    };

declare_event(peakGenerated, PeakInfo)
declare_event(peakExtrapolated, PeakInfo)
declare_event(contouringAt, int)
declare_event(contouringAt, PeakInfo)

public:
    Mapper();

    void generatePeaks(
        std::vector<PeakInfo> &peaks,
        PeakGenerationOptions opts);
    void extrapolatePeaks(
        Landscape &ls,
        const std::vector<PeakInfo> &peaks);
    void calculateContours(
        const Landscape &ls,
        const std::vector<int> &levelLayout,
        std::list<Line2dSegment> &contours);

    ~Mapper();

private:
    DISABLE_COPY(Mapper)
    DISABLE_MOVE(Mapper)
};


} // namespace HeightMap


#endif // _Mapper_h_
