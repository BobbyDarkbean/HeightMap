#include "terrain.h"
#include "../heightmaplogic.h"
#include "../preferences.h"
#include "../extrapolation/extrapolationdata.h"

#include "generatecommand.h"


namespace HeightMap {


struct GenerateCommandImplementation
{
    GenerateCommandImplementation();
    ~GenerateCommandImplementation();

    HeightMapLogic *logic;

    std::vector<PeakInfo> prevPeaks;
    std::vector<PeakInfo> nextPeaks;
    Preferences prevPrefs;
    Preferences nextPrefs;
    ExtrapolationData prevXData;
    ExtrapolationData nextXData;

private:
    DISABLE_COPY(GenerateCommandImplementation)
    DISABLE_MOVE(GenerateCommandImplementation)
};


GenerateCommandImplementation::GenerateCommandImplementation()
    : logic(nullptr),
      prevPrefs(),
      nextPrefs(),
      prevXData(),
      nextXData() { }

GenerateCommandImplementation::~GenerateCommandImplementation() { }


GenerateCommand::GenerateCommand(QUndoCommand *parent)
    : QUndoCommand(parent),
      m(new GenerateCommandImplementation) { }


void GenerateCommand::init(HeightMapLogic *l)
{
    m->logic = l;
    m->prevPrefs = l->preferences();
    m->prevXData = l->xData();
    m->prevPeaks = l->terrain()->peaks();
}

HeightMapLogic *GenerateCommand::logic() const
{ return m->logic; }

Preferences GenerateCommand::preferences() const
{ return m->nextPrefs; }

void GenerateCommand::setPreferences(const Preferences &prefs)
{ m->nextPrefs = prefs; }

ExtrapolationData GenerateCommand::xData() const
{ return m->nextXData; }

void GenerateCommand::setXData(const ExtrapolationData &xdata)
{ m->nextXData = xdata; }


void GenerateCommand::undo()
{
    m->nextPeaks = m->logic->terrain()->peaks();
    m->logic->terrain()->setPeaks(m->prevPeaks);

    m->logic->setPreferences(m->prevPrefs);
    m->logic->setXData(m->prevXData);
    m->logic->loadTerrain();
}

void GenerateCommand::redo()
{
    m->logic->setPreferences(m->nextPrefs);
    m->logic->setXData(m->nextXData);

    if (m->nextPeaks.empty()) {
        m->logic->createLandscape();
    } else {
        m->logic->terrain()->setPeaks(m->nextPeaks);
        m->logic->loadTerrain();
    }
}


GenerateCommand::~GenerateCommand()
{
    delete m;
}


} // namespace HeightMap
