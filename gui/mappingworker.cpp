#include <QImage>
#include <QMutex>
#include "auxiliary/mappingdata.h"
#include "heightmaplogic.h"
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

    HeightMapLogic *logic;

    MappingData data;
    Mapper mapper;
    QMutex mutex;

private:
    DISABLE_COPY(MappingWorkerImplementation)
    DISABLE_MOVE(MappingWorkerImplementation)
};


MappingWorkerImplementation::MappingWorkerImplementation()
    : logic(nullptr),
      data(),
      mapper(),
      mutex() { }

PeakGenerationOptions MappingWorkerImplementation::genOptions() const
{
    const Preferences &prefs = logic->preferences();
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
    int minLvl = logic->preferences().minContouringLevel();
    int maxLvl = logic->preferences().maxContouringLevel();
    int step = logic->preferences().contouringStep();

    for (int i = minLvl; i <= maxLvl; i += step)
        levels.push_back(i);
}

void MappingWorkerImplementation::drawPeaks()
{
    Terrain *terrain = logic->terrain();
    Engraver engr;
    int imageFactor = logic->preferences().imageFactor();

    QImage imgPk(terrain->width() * imageFactor,
                 terrain->height() * imageFactor,
                 QImage::Format_ARGB32_Premultiplied);
    engr.drawPeaks(terrain->peaks(), &imgPk, imageFactor);

    *data.imgPeaks = imgPk;
}

void MappingWorkerImplementation::drawLandscape()
{
    Terrain *terrain = logic->terrain();
    Engraver engr;
    int imageFactor = logic->preferences().imageFactor();

    QImage imgLs(terrain->width() * imageFactor,
                 terrain->height() * imageFactor,
                 QImage::Format_ARGB32_Premultiplied);
    engr.drawLandscape(terrain->landscape(), &imgLs, imageFactor);

    *data.imgLandscape = imgLs;
}

void MappingWorkerImplementation::drawIsobars()
{
    Terrain *terrain = logic->terrain();
    Engraver engr;
    int imageFactor = logic->preferences().imageFactor();

    QImage imgBars(terrain->width() * imageFactor,
                   terrain->height() * imageFactor,
                   QImage::Format_ARGB32_Premultiplied);
    engr.drawIsobars(terrain->contours(), &imgBars, true, imageFactor);

    *data.imgIsobars = imgBars;
}

void MappingWorkerImplementation::drawHybrid()
{
    Terrain *terrain = logic->terrain();
    Engraver engr;
    int imageFactor = logic->preferences().imageFactor();

    QImage imgHyb(terrain->width() * imageFactor,
                  terrain->height() * imageFactor,
                  QImage::Format_ARGB32_Premultiplied);
    engr.drawLandscape(terrain->landscape(), &imgHyb, imageFactor);
    engr.drawIsobars(terrain->contours(), &imgHyb, false, imageFactor);

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


void MappingWorker::bindLogic(HeightMapLogic *logic)
{ m->logic = logic; }

void MappingWorker::initFrom(const MappingData &data)
{ m->data = data; }


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

    Terrain *terrain = m->logic->terrain();
    terrain->clearPeaks();
    terrain->generatePeaks(&m->mapper, m->genOptions());
    m->drawPeaks();

    emit peakGeneratingFinished();
}

void MappingWorker::extrapolatePeaks()
{
    emit peakExtrapolationStarted();

    if (Extrapolator *extrapolator = m->logic->currentExtrapolator()) {
        Terrain *terrain = m->logic->terrain();
        terrain->fillLandscape(extrapolator->baseLevel());
        terrain->extrapolatePeaks(&m->mapper, extrapolator);
        m->drawLandscape();
    }

    emit peakExtrapolationFinished();
}

void MappingWorker::calculateContours()
{
    emit contouringStarted();

    std::vector<int> levels;
    m->fillLevels(levels);

    emit contouringLevelsAcquired(levels.size());

    Terrain *terrain = m->logic->terrain();
    terrain->clearContours();
    terrain->calculateContours(&m->mapper, levels);
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
