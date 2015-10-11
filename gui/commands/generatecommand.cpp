#include "../heightmaplogic.h"
#include "../preferences.h"
#include "../extrapolation/extrapolationdata.h"
#include "../trigger.h"

#include "generatecommand.h"


namespace HeightMap {


struct GenerateCommandImplementation
{
    GenerateCommandImplementation();
    ~GenerateCommandImplementation();

    HeightMapLogic *logic;

    Preferences prevPrefs;
    Preferences nextPrefs;
    ExtrapolationData prevXData;
    ExtrapolationData nextXData;

    Trigger *trg;

private:
    DISABLE_COPY(GenerateCommandImplementation)
    DISABLE_MOVE(GenerateCommandImplementation)
};


GenerateCommandImplementation::GenerateCommandImplementation()
    : logic(nullptr),
      prevPrefs(),
      nextPrefs(),
      prevXData(),
      nextXData(),
      trg(nullptr) { }

GenerateCommandImplementation::~GenerateCommandImplementation() { }


GenerateCommand::GenerateCommand(QUndoCommand *parent)
    : QUndoCommand(parent),
      m(new GenerateCommandImplementation) { }


HeightMapLogic *GenerateCommand::logic() const
{ return m->logic; }

void GenerateCommand::setLogic(HeightMapLogic *l)
{ m->logic = l; }

Preferences GenerateCommand::prevPreferences() const
{ return m->prevPrefs; }

void GenerateCommand::setPrevPreferences(const Preferences &prefs)
{ m->prevPrefs = prefs; }

Preferences GenerateCommand::nextPreferences() const
{ return m->nextPrefs; }

void GenerateCommand::setNextPreferences(const Preferences &prefs)
{ m->nextPrefs = prefs; }

ExtrapolationData GenerateCommand::prevXData() const
{ return m->prevXData; }

void GenerateCommand::setPrevXData(const ExtrapolationData &xdata)
{ m->prevXData = xdata; }

ExtrapolationData GenerateCommand::nextXData() const
{ return m->nextXData; }

void GenerateCommand::setNextXData(const ExtrapolationData &xdata)
{ m->nextXData = xdata; }

Trigger *GenerateCommand::trigger() const
{ return m->trg; }

void GenerateCommand::setTrigger(Trigger *trg)
{ m->trg = trg; }


void GenerateCommand::undo()
{
    m->logic->setPreferences(m->prevPrefs);
    m->logic->setXData(m->prevXData);
    m->trg->activate();
}

void GenerateCommand::redo()
{
    m->logic->setPreferences(m->nextPrefs);
    m->logic->setXData(m->nextXData);
    m->trg->activate();
}


GenerateCommand::~GenerateCommand()
{
    delete m;
}


} // namespace HeightMap
