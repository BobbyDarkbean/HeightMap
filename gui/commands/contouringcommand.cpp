#include "../heightmaplogic.h"
#include "../preferences.h"
#include "../extrapolation/extrapolationdata.h"
#include "../trigger.h"

#include "contouringcommand.h"


namespace HeightMap {


struct ContouringCommandImplementation
{
    ContouringCommandImplementation();
    ~ContouringCommandImplementation();

    HeightMapLogic *logic;

    Preferences prevPrefs;
    Preferences nextPrefs;
    ExtrapolationData prevXData;
    ExtrapolationData nextXData;

    Trigger *trg;

private:
    DISABLE_COPY(ContouringCommandImplementation)
    DISABLE_MOVE(ContouringCommandImplementation)
};


ContouringCommandImplementation::ContouringCommandImplementation()
    : logic(nullptr),
      prevPrefs(),
      nextPrefs(),
      prevXData(),
      nextXData(),
      trg(nullptr) { }

ContouringCommandImplementation::~ContouringCommandImplementation() { }


ContouringCommand::ContouringCommand(QUndoCommand *parent)
    : QUndoCommand(parent),
      m(new ContouringCommandImplementation) { }


HeightMapLogic *ContouringCommand::logic() const
{ return m->logic; }

void ContouringCommand::setLogic(HeightMapLogic *l)
{ m->logic = l; }

Preferences ContouringCommand::prevPreferences() const
{ return m->prevPrefs; }

void ContouringCommand::setPrevPreferences(const Preferences &prefs)
{ m->prevPrefs = prefs; }

Preferences ContouringCommand::nextPreferences() const
{ return m->nextPrefs; }

void ContouringCommand::setNextPreferences(const Preferences &prefs)
{ m->nextPrefs = prefs; }

ExtrapolationData ContouringCommand::prevXData() const
{ return m->prevXData; }

void ContouringCommand::setPrevXData(const ExtrapolationData &xdata)
{ m->prevXData = xdata; }

ExtrapolationData ContouringCommand::nextXData() const
{ return m->nextXData; }

void ContouringCommand::setNextXData(const ExtrapolationData &xdata)
{ m->nextXData = xdata; }

Trigger *ContouringCommand::trigger() const
{ return m->trg; }

void ContouringCommand::setTrigger(Trigger *trg)
{ m->trg = trg; }


void ContouringCommand::undo()
{
    m->logic->setPreferences(m->prevPrefs);
    m->logic->setXData(m->prevXData);
    m->trg->activate();
}

void ContouringCommand::redo()
{
    m->logic->setPreferences(m->nextPrefs);
    m->logic->setXData(m->nextXData);
    m->trg->activate();
}


ContouringCommand::~ContouringCommand()
{
    delete m;
}


} // namespace HeightMap
