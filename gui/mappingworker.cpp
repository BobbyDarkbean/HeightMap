#include <QMutex>
#include "mapper.h"
#include "mappingdata.h"

#include "mappingworker.h"


namespace HeightMap {


struct MappingWorkerImplementation
{
    MappingWorkerImplementation();
    ~MappingWorkerImplementation();

    MappingData data;
    Mapper mapper;
    QMutex mutex;

private:
    DISABLE_COPY(MappingWorkerImplementation)
    DISABLE_MOVE(MappingWorkerImplementation)
};


MappingWorkerImplementation::MappingWorkerImplementation()
    : data(),
      mapper(),
      mutex() { }

MappingWorkerImplementation::~MappingWorkerImplementation() { }


MappingWorker::MappingWorker(QObject *parent)
    : QObject(parent),
      m(new MappingWorkerImplementation),
      init_eventhandler(Mapper, peakExtrapolated, PeakInfo),
      init_eventhandler(Mapper, contouringAt, PeakInfo)
{
    handle_event(Mapper, (&m->mapper), this, peakExtrapolated, PeakInfo);
    handle_event(Mapper, (&m->mapper), this, contouringAt, PeakInfo);
}

void MappingWorker::initFrom(MappingData *data)
{
    m->data = *data;
}


MappingWorker::~MappingWorker()
{
    delete m;
}


void MappingWorker::createLandscape()
{
    QMutexLocker lock(&m->mutex);

    emit processStarted();

    generatePeaks();
    extrapolatePeaks();
    calculateContours();

    emit processFinished();
}


void MappingWorker::generatePeaks()
{
    m->mapper.generatePeaks(*m->data.peaks, *m->data.genOptions);
    emit peakGeneratingFinished();
}

void MappingWorker::extrapolatePeaks()
{
    m->mapper.extrapolatePeaks(*m->data.landscape, *m->data.peaks);
    emit peakExtrapolationFinished();
}

void MappingWorker::calculateContours()
{
    m->mapper.calculateContours(*m->data.landscape, *m->data.levels, *m->data.contours);
    emit contouringFinished();
}


implement_eventhandler(MappingWorker, Mapper, peakExtrapolated, PeakInfo)
{
    QPoint coords(args.x, args.y);
    master()->_emit_peakExtrapolated(coords, args.height);
}

implement_eventhandler(MappingWorker, Mapper, contouringAt, PeakInfo)
{
    QPoint coords(args.x, args.y);
    master()->_emit_contouringAt(coords);
}


} // namespace HeightMap
