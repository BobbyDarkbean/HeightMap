#include "../heightmaplogic.h"
#include "preferences.h"
#include "extrapolationdata.h"

#include "extrapolatecommand.h"


namespace HeightMap {


struct ExtrapolateCommandImplementation
{
    ExtrapolateCommandImplementation();
    ~ExtrapolateCommandImplementation();

    HeightMapLogic *logic;

    Preferences prevPrefs;
    Preferences nextPrefs;
    ExtrapolationData prevXData;
    ExtrapolationData nextXData;

private:
    DISABLE_COPY(ExtrapolateCommandImplementation)
    DISABLE_MOVE(ExtrapolateCommandImplementation)
};


ExtrapolateCommandImplementation::ExtrapolateCommandImplementation()
    : logic(nullptr),
      prevPrefs(),
      nextPrefs(),
      prevXData(),
      nextXData() { }

ExtrapolateCommandImplementation::~ExtrapolateCommandImplementation() { }


ExtrapolateCommand::ExtrapolateCommand(QUndoCommand *parent)
    : QUndoCommand(parent),
      m(new ExtrapolateCommandImplementation) { }


void ExtrapolateCommand::init(HeightMapLogic *l)
{
    m->logic = l;
    m->prevPrefs = l->preferences();
    m->prevXData = l->xData();
}

HeightMapLogic *ExtrapolateCommand::logic() const
{ return m->logic; }

Preferences ExtrapolateCommand::preferences() const
{ return m->nextPrefs; }

void ExtrapolateCommand::setPreferences(const Preferences &prefs)
{ m->nextPrefs = prefs; }

ExtrapolationData ExtrapolateCommand::xData() const
{ return m->nextXData; }

void ExtrapolateCommand::setXData(const ExtrapolationData &xdata)
{ m->nextXData = xdata; }


void ExtrapolateCommand::undo()
{
    m->logic->setPreferences(m->prevPrefs);
    m->logic->setXData(m->prevXData);
    m->logic->buildLandscapeFromPeaks();
}

void ExtrapolateCommand::redo()
{
    m->logic->setPreferences(m->nextPrefs);
    m->logic->setXData(m->nextXData);
    m->logic->buildLandscapeFromPeaks();
}


ExtrapolateCommand::~ExtrapolateCommand()
{
    delete m;
}


} // namespace HeightMap
