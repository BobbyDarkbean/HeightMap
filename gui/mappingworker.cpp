#include <QImage>
#include <QMutex>
#include "terrain.h"
#include "mapper.h"
#include "mappingdata.h"
#include "engraver.h"

#include "mappingworker.h"


namespace HeightMap {


struct MappingWorkerImplementation
{
    MappingWorkerImplementation();

    void drawLandscape();
    void drawIsobars();
    void drawHybrid();

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

void MappingWorkerImplementation::drawLandscape()
{
    Engraver engr;
    QImage imgLs(data.terrain->width() * data.imageFactor,
                 data.terrain->height() * data.imageFactor,
                 QImage::Format_ARGB32_Premultiplied);
    engr.drawLandscape(data.terrain->landscape(), &imgLs, data.imageFactor);

    *data.imgLandscape = imgLs;
}

void MappingWorkerImplementation::drawIsobars()
{
    Engraver engr;
    QImage imgBars(data.terrain->width() * data.imageFactor,
                   data.terrain->height() * data.imageFactor,
                   QImage::Format_ARGB32_Premultiplied);
    engr.drawIsobars(data.terrain->contours(), &imgBars, true, data.imageFactor);

    *data.imgIsobars = imgBars;
}

void MappingWorkerImplementation::drawHybrid()
{
    Engraver engr;
    QImage imgHyb(data.terrain->width() * data.imageFactor,
                  data.terrain->height() * data.imageFactor,
                  QImage::Format_ARGB32_Premultiplied);
    engr.drawLandscape(data.terrain->landscape(), &imgHyb, data.imageFactor);
    engr.drawIsobars(data.terrain->contours(), &imgHyb, false, data.imageFactor);

    *data.imgHybrid = imgHyb;
}

MappingWorkerImplementation::~MappingWorkerImplementation() { }


MappingWorker::MappingWorker(QObject *parent)
    : QObject(parent),
      m(new MappingWorkerImplementation),
      init_eventhandler(Mapper, peakExtrapolated, PeakInfo),
      init_eventhandler(Mapper, contouringAt, int)
{
    handle_event(Mapper, (&m->mapper), this, peakExtrapolated, PeakInfo);
    handle_event(Mapper, (&m->mapper), this, contouringAt, int);
}

void MappingWorker::initFrom(const MappingData &data)
{
    m->data = data;
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

void MappingWorker::buildLandscapeFromPeaks()
{
    QMutexLocker lock(&m->mutex);

    emit processStarted();

    extrapolatePeaks();
    calculateContours();

    emit processFinished();
}


void MappingWorker::generatePeaks()
{
    emit peakGeneratingStarted();

    m->data.terrain->clearPeaks();
    m->data.terrain->generatePeaks(&m->mapper, *m->data.genOptions);

    emit peakGeneratingFinished();
}

void MappingWorker::extrapolatePeaks()
{
    emit peakExtrapolationStarted();

    m->data.terrain->fillLandscape(m->data.genOptions->baseLvl);
    m->data.terrain->extrapolatePeaks(&m->mapper, m->data.genOptions->baseLvl);
    m->drawLandscape();

    emit peakExtrapolationFinished();
}

void MappingWorker::calculateContours()
{
    emit contouringStarted();

    m->data.terrain->clearContours();
    m->data.terrain->calculateContours(&m->mapper, *m->data.levels);
    m->drawIsobars();
    m->drawHybrid();

    emit contouringFinished();
}


implement_eventhandler(MappingWorker, Mapper, peakExtrapolated, PeakInfo)
{
    QPoint coords(args.x, args.y);
    master()->_emit_peakExtrapolated(coords, args.height);
}

implement_eventhandler(MappingWorker, Mapper, contouringAt, int)
{
    master()->_emit_contouringAt(args);
}


} // namespace HeightMap
