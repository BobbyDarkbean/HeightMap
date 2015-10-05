#include <memory>

#include <QMap>
#include <QImage>
#include "terrain.h"
#include "extrapolation/extrapolationfactory.h"
#include "auxiliary/mappingdata.h"
#include "mappingthread.h"
#include "mappingworker.h"
#include "preferences.h"
#include "preferencescontroller.h"
#include "trigger.h"
#include "heightmapapplication.h"

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
    PreferencesController *ctrl;
    QMap<QString, ExtrapolationFactory *> extrapolations;

    QImage imgPeaks;
    QImage imgLandscape;
    QImage imgIsobars;
    QImage imgHybrid;

    MappingThread *thrProcess;

    Trigger *trgGenLs;
    Trigger *trgBuildLs;
    Trigger *trgCalcContours;

private:
    DISABLE_COPY(HeightMapLogicImplementation)
    DISABLE_MOVE(HeightMapLogicImplementation)
};


HeightMapLogicImplementation::HeightMapLogicImplementation()
    : terrain(nullptr),
      prefs(),
      ctrl(nullptr),
      extrapolations(),
      imgPeaks(),
      imgLandscape(),
      imgIsobars(),
      imgHybrid(),
      thrProcess(nullptr),
      trgGenLs(nullptr),
      trgBuildLs(nullptr),
      trgCalcContours(nullptr) { }

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

    for (auto i = extrapolations.constBegin(); i != extrapolations.constEnd(); ++i) {
        delete i.value();
    }
}


HeightMapLogic::HeightMapLogic(QObject *parent)
    : QObject(parent),
      m(new HeightMapLogicImplementation)
{
    m->ctrl = new PreferencesController(this);
    m->ctrl->setPreferences(&m->prefs);

    m->thrProcess = new MappingThread(this);

    MappingWorker *worker = new MappingWorker;
    m->initWorker(worker);
    worker->moveToThread(m->thrProcess);

    m->trgGenLs = new Trigger(this);
    m->trgBuildLs = new Trigger(this);
    m->trgCalcContours = new Trigger(this);

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
{
    if (m->prefs != prefs) {
        m->prefs = prefs;
        emit preferencesChanged();
    }
}

PreferencesController *HeightMapLogic::preferencesController() const
{ return m->ctrl; }


void HeightMapLogic::addExtrapolation(ExtrapolationFactory *f)
{ m->extrapolations.insert(f->name(), f); }

QStringList HeightMapLogic::extrapolatorKeys() const
{ return m->extrapolations.keys(); }

ExtrapolationFactory *HeightMapLogic::extrapolationFactory(const QString &name) const
{ return m->extrapolations.value(name, nullptr); }

Extrapolator *HeightMapLogic::currentExtrapolator() const
{
    QString currentName = preferences().extrapolatorName();
    if (ExtrapolationFactory *f = extrapolationFactory(currentName)) {
        return f->extrapolator();
    }

    return nullptr;
}

void HeightMapLogic::applyProxyExtrapolator(const QString &name)
{
    for (auto i = m->extrapolations.constBegin(); i != m->extrapolations.constEnd(); ++i) {
        if (ExtrapolationFactory *f = i.value()) {
            if (i.key() == name) {
                f->applyProxyData();
                emit extrapolationDataChanged(name);
            } else {
                f->resetProxyData();
            }
        }
    }
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
    int w = hmApp->preferences().landscapeWidth();
    int h = hmApp->preferences().landscapeHeight();

    m->terrain.reset(new Terrain(w, h));    
    m->resetImages();

    emit terrainCreated();
}

void HeightMapLogic::createLandscape()
{ m->trgGenLs->activate(); }

void HeightMapLogic::buildLandscapeFromPeaks()
{ m->trgBuildLs->activate(); }

void HeightMapLogic::plotIsobars()
{ m->trgCalcContours->activate(); }


} // namespace HeightMap
