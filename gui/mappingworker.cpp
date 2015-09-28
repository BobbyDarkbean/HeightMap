#include <QImage>
#include <QMutex>
#include "auxiliary/mappingdata.h"
#include "heightmapapplication.h"
#include "preferences.h"
#include "terrain.h"
#include "mapper.h"
#include "extrapolator.h"
#include "engraver.h"

#include "mappingworker.h"


namespace HeightMap {


struct MappingWorkerImplementation
{
    MappingWorkerImplementation();

    PeakGenerationOptions genOptions() const;
    void fillLevels(std::vector<int> &levels);

    void drawPeaks();
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

PeakGenerationOptions MappingWorkerImplementation::genOptions() const
{
    const Preferences &prefs = hmApp->preferences();
    return PeakGenerationOptions {
        prefs.peakCount(),
        prefs.landscapeWidth(),
        prefs.landscapeHeight(),
        prefs.minPeak(),
        prefs.maxPeak()
    };
}

void MappingWorkerImplementation::fillLevels(std::vector<int> &levels)
{
    int minLvl = hmApp->preferences().minContouringLevel();
    int maxLvl = hmApp->preferences().maxContouringLevel();
    int step = hmApp->preferences().contouringStep();

    for (int i = minLvl; i <= maxLvl; i += step)
        levels.push_back(i);
}

void MappingWorkerImplementation::drawPeaks()
{
    Engraver engr;
    int imageFactor = hmApp->preferences().imageFactor();

    QImage imgPk(data.terrain->width() * imageFactor,
                 data.terrain->height() * imageFactor,
                 QImage::Format_ARGB32_Premultiplied);
    engr.drawPeaks(data.terrain->peaks(), &imgPk, imageFactor);

    *data.imgPeaks = imgPk;
}

void MappingWorkerImplementation::drawLandscape()
{
    Engraver engr;
    int imageFactor = hmApp->preferences().imageFactor();

    QImage imgLs(data.terrain->width() * imageFactor,
                 data.terrain->height() * imageFactor,
                 QImage::Format_ARGB32_Premultiplied);
    engr.drawLandscape(data.terrain->landscape(), &imgLs, imageFactor);

    *data.imgLandscape = imgLs;
}

void MappingWorkerImplementation::drawIsobars()
{
    Engraver engr;
    int imageFactor = hmApp->preferences().imageFactor();

    QImage imgBars(data.terrain->width() * imageFactor,
                   data.terrain->height() * imageFactor,
                   QImage::Format_ARGB32_Premultiplied);
    engr.drawIsobars(data.terrain->contours(), &imgBars, true, imageFactor);

    *data.imgIsobars = imgBars;
}

void MappingWorkerImplementation::drawHybrid()
{
    Engraver engr;
    int imageFactor = hmApp->preferences().imageFactor();

    QImage imgHyb(data.terrain->width() * imageFactor,
                  data.terrain->height() * imageFactor,
                  QImage::Format_ARGB32_Premultiplied);
    engr.drawLandscape(data.terrain->landscape(), &imgHyb, imageFactor);
    engr.drawIsobars(data.terrain->contours(), &imgHyb, false, imageFactor);

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

void MappingWorker::plotIsobars()
{
    QMutexLocker lock(&m->mutex);

    emit processStarted();

    calculateContours();

    emit processFinished();
}


void MappingWorker::generatePeaks()
{
    emit peakGeneratingStarted();

    m->data.terrain->clearPeaks();
    m->data.terrain->generatePeaks(&m->mapper, m->genOptions());
    m->drawPeaks();

    emit peakGeneratingFinished();
}

void MappingWorker::extrapolatePeaks()
{
    emit peakExtrapolationStarted();

    m->data.terrain->fillLandscape((new SimpleExtrapolator)->baseLevel());
    m->data.terrain->extrapolatePeaks(&m->mapper, new SimpleExtrapolator);
    m->drawLandscape();

    emit peakExtrapolationFinished();
}

void MappingWorker::calculateContours()
{
    emit contouringStarted();

    std::vector<int> levels;
    m->fillLevels(levels);

    emit contouringLevelsAcquired(levels.size());

    m->data.terrain->clearContours();
    m->data.terrain->calculateContours(&m->mapper, levels);
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
