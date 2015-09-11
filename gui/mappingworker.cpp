#include "mapper.h"
#include "mappingworker.h"


namespace HeightMap {


MappingWorker::MappingWorker(QObject *parent)
    : QObject(parent),
      init_eventhandler(Mapper, peakExtrapolated, PeakInfo),
      init_eventhandler(Mapper, contouringAt, Line2dSegment)
{
}


MappingWorker::~MappingWorker()
{
}


void MappingWorker::generatePeaks()
{
}

void MappingWorker::extrapolatePeaks()
{
}

void MappingWorker::calculateContours()
{
}


implement_eventhandler(MappingWorker, Mapper, peakExtrapolated, PeakInfo)
{
    QPoint coords(args.x, args.y);
    master()->_emit_peakExtrapolated(coords, args.height);
}

implement_eventhandler(MappingWorker, Mapper, contouringAt, Line2dSegment)
{
    QLine segment(args.x1, args.y1, args.x2, args.y2);
    master()->_emit_contouringAt(segment, args.level);
}


} // namespace HeightMap
