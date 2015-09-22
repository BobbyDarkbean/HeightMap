#include <QImage>
#include <QMutex>
#include "landscape.h"
#include "mapper.h"
#include "mappingdata.h"
#include "engraver.h"

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
    drawOffScreenRepresentation();

    emit processFinished();
}


void MappingWorker::generatePeaks()
{
    emit peakGeneratingStarted();

    m->data.landscape->fill(64);
    m->mapper.generatePeaks(*m->data.peaks, *m->data.genOptions);

    emit peakGeneratingFinished();
}

void MappingWorker::extrapolatePeaks()
{
    emit peakExtrapolationStarted();
    m->mapper.extrapolatePeaks(*m->data.landscape, *m->data.peaks, m->data.genOptions->baseLvl);
    emit peakExtrapolationFinished();
}

void MappingWorker::calculateContours()
{
    emit contouringStarted();
    m->mapper.calculateContours(*m->data.landscape, *m->data.levels, *m->data.contours);
    emit contouringFinished();
}

void MappingWorker::drawOffScreenRepresentation()
{
    emit offScreenDrawingStarted();

    Engraver engr;
    const int ImageFactor = 4;

    QImage imgLs(m->data.landscape->width() * ImageFactor,
                 m->data.landscape->height() * ImageFactor,
                 QImage::Format_ARGB32_Premultiplied);
    engr.drawLandscape(*m->data.landscape, &imgLs, ImageFactor);
    QImage imgBars(m->data.landscape->width() * ImageFactor,
                   m->data.landscape->height() * ImageFactor,
                   QImage::Format_ARGB32_Premultiplied);
    engr.drawIsobars(*m->data.contours, &imgBars, true, ImageFactor);
    QImage imgHyb(m->data.landscape->width() * ImageFactor,
                  m->data.landscape->height() * ImageFactor,
                  QImage::Format_ARGB32_Premultiplied);
    engr.drawLandscape(*m->data.landscape, &imgHyb, ImageFactor);
    engr.drawIsobars(*m->data.contours, &imgHyb, false, ImageFactor);

    *m->data.imgLandscape = imgLs;
    *m->data.imgIsobars = imgBars;
    *m->data.imgHybrid = imgHyb;

    emit offScreenDrawingFinished();
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
