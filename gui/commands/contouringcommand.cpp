#include "../heightmaplogic.h"
#include "../preferences.h"
#include "../extrapolation/extrapolationdata.h"

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

private:
    DISABLE_COPY(ContouringCommandImplementation)
    DISABLE_MOVE(ContouringCommandImplementation)
};


ContouringCommandImplementation::ContouringCommandImplementation()
    : logic(nullptr),
      prevPrefs(),
      nextPrefs(),
      prevXData(),
      nextXData() { }

ContouringCommandImplementation::~ContouringCommandImplementation() { }


ContouringCommand::ContouringCommand(QUndoCommand *parent)
    : QUndoCommand(parent),
      m(new ContouringCommandImplementation) { }


void ContouringCommand::init(HeightMapLogic *l)
{
    m->logic = l;
    m->prevPrefs = l->preferences();
    m->prevXData = l->xData();
}

HeightMapLogic *ContouringCommand::logic() const
{ return m->logic; }

Preferences ContouringCommand::preferences() const
{ return m->nextPrefs; }

void ContouringCommand::setPreferences(const Preferences &prefs)
{ m->nextPrefs = prefs; }

ExtrapolationData ContouringCommand::xData() const
{ return m->nextXData; }

void ContouringCommand::setXData(const ExtrapolationData &xdata)
{ m->nextXData = xdata; }


void ContouringCommand::undo()
{
    m->logic->setPreferences(m->prevPrefs);
    m->logic->setXData(m->prevXData);
    m->logic->plotIsobars();
}

void ContouringCommand::redo()
{
    m->logic->setPreferences(m->nextPrefs);
    m->logic->setXData(m->nextXData);
    m->logic->plotIsobars();
}


ContouringCommand::~ContouringCommand()
{
    delete m;
}


} // namespace HeightMap
