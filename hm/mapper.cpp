#include "mapper.h"


namespace HeightMap {


Mapper::Mapper()
    : init_event(peakGenerated, PeakInfo),
      init_event(peakExtrapolated, PeakInfo),
      init_event(contouringAt, PeakInfo) { }


void Mapper::generatePeaks(
    std::vector<PeakInfo> &peaks,
    const PeakGenerationOptions &opts)
{

}


void Mapper::extrapolatePeaks(
    Landscape &ls,
    const std::vector<PeakInfo> &peaks)
{

}


void Mapper::calculateContours(
    const Landscape &ls,
    const std::vector<int> &levelLayout,
    std::list<Line2dSegment> &contours)
{

}


Mapper::~Mapper() { }


} // namespace HeightMap
