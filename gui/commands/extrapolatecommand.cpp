#include "../heightmaplogic.h"
#include "../preferences.h"
#include "../extrapolation/extrapolationdata.h"
#include "../trigger.h"

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

    Trigger *trg;

private:
    DISABLE_COPY(ExtrapolateCommandImplementation)
    DISABLE_MOVE(ExtrapolateCommandImplementation)
};


ExtrapolateCommandImplementation::ExtrapolateCommandImplementation()
    : logic(nullptr),
      prevPrefs(),
      nextPrefs(),
      prevXData(),
      nextXData(),
      trg(nullptr) { }

ExtrapolateCommandImplementation::~ExtrapolateCommandImplementation() { }


ExtrapolateCommand::ExtrapolateCommand(QUndoCommand *parent)
    : QUndoCommand(parent),
      m(new ExtrapolateCommandImplementation) { }


HeightMapLogic *ExtrapolateCommand::logic() const
{ return m->logic; }

void ExtrapolateCommand::setLogic(HeightMapLogic *l)
{ m->logic = l; }

Preferences ExtrapolateCommand::prevPreferences() const
{ return m->prevPrefs; }

void ExtrapolateCommand::setPrevPreferences(const Preferences &prefs)
{ m->prevPrefs = prefs; }

Preferences ExtrapolateCommand::nextPreferences() const
{ return m->nextPrefs; }

void ExtrapolateCommand::setNextPreferences(const Preferences &prefs)
{ m->nextPrefs = prefs; }

ExtrapolationData ExtrapolateCommand::prevXData() const
{ return m->prevXData; }

void ExtrapolateCommand::setPrevXData(const ExtrapolationData &xdata)
{ m->prevXData = xdata; }

ExtrapolationData ExtrapolateCommand::nextXData() const
{ return m->nextXData; }

void ExtrapolateCommand::setNextXData(const ExtrapolationData &xdata)
{ m->nextXData = xdata; }

Trigger *ExtrapolateCommand::trigger() const
{ return m->trg; }

void ExtrapolateCommand::setTrigger(Trigger *trg)
{ m->trg = trg; }


void ExtrapolateCommand::undo()
{
    m->logic->setPreferences(m->prevPrefs);
    m->logic->setXData(m->prevXData);
    m->trg->activate();
}

void ExtrapolateCommand::redo()
{
    m->logic->setPreferences(m->nextPrefs);
    m->logic->setXData(m->nextXData);
    m->trg->activate();
}


ExtrapolateCommand::~ExtrapolateCommand()
{
    delete m;
}


} // namespace HeightMap
