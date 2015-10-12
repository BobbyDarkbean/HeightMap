#include <memory>

#include <QMap>
#include <QImage>
#include "terrain.h"
#include "extrapolation/extrapolationdata.h"
#include "extrapolation/extrapolationfactory.h"
#include "auxiliary/mappingdata.h"
#include "commands/commands"
#include "heightmapapplication.h"
#include "mappingthread.h"
#include "mappingworker.h"
#include "preferences.h"
#include "preferencescontroller.h"
#include "trigger.h"

#include "heightmaplogic.h"


namespace HeightMap {


struct HeightMapLogicImplementation
{
    HeightMapLogicImplementation();

    void initWorker(MappingWorker *worker);
    void resetImages();

    ~HeightMapLogicImplementation();

    std::unique_ptr<Terrain> terrain;

    Preferences prefs;
    ExtrapolationData xData;

    QImage imgPeaks;
    QImage imgLandscape;
    QImage imgIsobars;
    QImage imgHybrid;

    MappingThread *thrProcess;

    Trigger *trgGenLs;
    Trigger *trgBuildLs;
    Trigger *trgCalcContours;

    QUndoStack *uskCommands;

private:
    DISABLE_COPY(HeightMapLogicImplementation)
    DISABLE_MOVE(HeightMapLogicImplementation)
};


HeightMapLogicImplementation::HeightMapLogicImplementation()
    : terrain(nullptr),
      prefs(),
      xData(),
      imgPeaks(),
      imgLandscape(),
      imgIsobars(),
      imgHybrid(),
      thrProcess(nullptr),
      trgGenLs(nullptr),
      trgBuildLs(nullptr),
      trgCalcContours(nullptr),
      uskCommands(nullptr) { }

void HeightMapLogicImplementation::initWorker(MappingWorker *worker)
{
    MappingData hmData = {
        &imgPeaks,
        &imgLandscape,
        &imgIsobars,
        &imgHybrid
    };

    worker->initFrom(hmData);
}

void HeightMapLogicImplementation::resetImages()
{
    imgPeaks = QImage(terrain->width(), terrain->height(), QImage::Format_ARGB32_Premultiplied);
    imgPeaks.fill(Qt::transparent);

    imgLandscape = QImage(terrain->width(), terrain->height(), QImage::Format_ARGB32_Premultiplied);
    imgLandscape.fill(Qt::transparent);

    imgIsobars = QImage(terrain->width(), terrain->height(), QImage::Format_ARGB32_Premultiplied);
    imgIsobars.fill(Qt::transparent);

    imgHybrid = QImage(terrain->width(), terrain->height(), QImage::Format_ARGB32_Premultiplied);
    imgHybrid.fill(Qt::transparent);
}

HeightMapLogicImplementation::~HeightMapLogicImplementation()
{
    thrProcess->quit();
    thrProcess->wait();
}


HeightMapLogic::HeightMapLogic(QObject *parent)
    : QObject(parent),
      m(new HeightMapLogicImplementation)
{
    m->thrProcess = new MappingThread(this);

    MappingWorker *worker = new MappingWorker;
    m->initWorker(worker);
    worker->bindLogic(this);
    worker->moveToThread(m->thrProcess);

    m->trgGenLs = new Trigger(this);
    m->trgBuildLs = new Trigger(this);
    m->trgCalcContours = new Trigger(this);

    m->uskCommands = new QUndoStack(this);

    typedef MappingWorker W;
    typedef HeightMapLogic L;
    typedef Trigger T;
    typedef MappingThread M;

    connect(m->trgGenLs,        &T::activated,      worker, &W::createLandscape);
    connect(m->trgBuildLs,      &T::activated,      worker, &W::buildLandscapeFromPeaks);
    connect(m->trgCalcContours, &T::activated,      worker, &W::plotIsobars);

    connect(worker, &W::processStarted,             this,   &L::processStarted,             Qt::BlockingQueuedConnection);
    connect(worker, &W::processFinished,            this,   &L::processFinished,            Qt::BlockingQueuedConnection);
    connect(worker, &W::peakGeneratingStarted,      this,   &L::peakGeneratingStarted,      Qt::BlockingQueuedConnection);
    connect(worker, &W::peakGeneratingFinished,     this,   &L::peakGeneratingFinished,     Qt::BlockingQueuedConnection);
    connect(worker, &W::peakExtrapolationStarted,   this,   &L::peakExtrapolationStarted,   Qt::BlockingQueuedConnection);
    connect(worker, &W::peakExtrapolated,           this,   &L::peakExtrapolated);
    connect(worker, &W::peakExtrapolationFinished,  this,   &L::peakExtrapolationFinished,  Qt::BlockingQueuedConnection);
    connect(worker, &W::contouringStarted,          this,   &L::contouringStarted,          Qt::BlockingQueuedConnection);
    connect(worker, &W::contouringLevelsAcquired,   this,   &L::contouringLevelsAcquired,   Qt::BlockingQueuedConnection);
    connect(worker, &W::contouringAt,               this,   &L::contouringAt);
    connect(worker, &W::contouringFinished,         this,   &L::contouringFinished,         Qt::BlockingQueuedConnection);

    connect(m->thrProcess,      &M::finished,       worker, &W::deleteLater);
    m->thrProcess->start();
}


Terrain *HeightMapLogic::terrain()
{ return m->terrain.get(); }

const Terrain *HeightMapLogic::terrain() const
{ return m->terrain.get(); }

const Preferences &HeightMapLogic::preferences() const
{ return m->prefs; }

void HeightMapLogic::setPreferences(const Preferences &prefs)
{ m->prefs = prefs; }

ExtrapolationData HeightMapLogic::xData() const
{ return m->xData; }

void HeightMapLogic::setXData(const ExtrapolationData &data)
{ m->xData = data; }


ExtrapolationFactory *HeightMapLogic::currentExtrapolation() const
{
    QString currentName = preferences().extrapolatorName();
    return hmApp->extrapolationFactory(currentName);
}


const QImage &HeightMapLogic::heightMapImage(HeightMapViewMode hmvm)
{
    switch (hmvm) {
    default:
    case HMVM_Peaks:
        return m->imgPeaks;
    case HMVM_Landscape:
        return m->imgLandscape;
    case HMVM_Isobars:
        return m->imgIsobars;
    case HMVM_Hybrid:
        return m->imgHybrid;
    }
}


HeightMapLogic::~HeightMapLogic()
{
    delete m;
}


void HeightMapLogic::newTerrain()
{
    m->uskCommands->clear();

    int w = m->prefs.landscapeWidth();
    int h = m->prefs.landscapeHeight();

    m->terrain.reset(new Terrain(w, h));    
    m->resetImages();

    emit terrainCreated();
}

void HeightMapLogic::createLandscape()
{
    GenerateCommand *cmd = new GenerateCommand;
    cmd->setText(tr("Generate peaks"));
    cmd->setLogic(this);
    cmd->setPrevPreferences(m->prefs);
    cmd->setPrevXData(m->xData);
    cmd->setNextPreferences(hmApp->preferences());
    if (ExtrapolationFactory *f = currentExtrapolation()) {
        cmd->setNextXData(f->extractData());
    }
    cmd->setTrigger(m->trgGenLs);

    m->uskCommands->push(cmd);
    m->uskCommands->redo();
}

void HeightMapLogic::buildLandscapeFromPeaks()
{
    ExtrapolateCommand *cmd = new ExtrapolateCommand;
    cmd->setText(tr("Extrapolate peaks"));
    cmd->setLogic(this);
    cmd->setPrevPreferences(m->prefs);
    cmd->setPrevXData(m->xData);
    cmd->setNextPreferences(hmApp->preferences());
    if (ExtrapolationFactory *f = currentExtrapolation()) {
        cmd->setNextXData(f->extractData());
    }
    cmd->setTrigger(m->trgBuildLs);

    m->uskCommands->push(cmd);
    m->uskCommands->redo();
}

void HeightMapLogic::plotIsobars()
{
    ContouringCommand *cmd = new ContouringCommand;
    cmd->setText(tr("Calculate isobars"));
    cmd->setLogic(this);
    cmd->setPrevPreferences(m->prefs);
    cmd->setPrevXData(m->xData);
    cmd->setNextPreferences(hmApp->preferences());
    if (ExtrapolationFactory *f = currentExtrapolation()) {
        cmd->setNextXData(f->extractData());
    }
    cmd->setTrigger(m->trgCalcContours);

    m->uskCommands->push(cmd);
    m->uskCommands->redo();
}


} // namespace HeightMap
