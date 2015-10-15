#include "../heightmaplogic.h"
#include "preferences.h"

#include "contouringcommand.h"


namespace HeightMap {


struct ContouringCommandImplementation
{
    ContouringCommandImplementation();
    ~ContouringCommandImplementation();

    HeightMapLogic *logic;

    Preferences prevPrefs;
    Preferences nextPrefs;

private:
    DISABLE_COPY(ContouringCommandImplementation)
    DISABLE_MOVE(ContouringCommandImplementation)
};


ContouringCommandImplementation::ContouringCommandImplementation()
    : logic(nullptr),
      prevPrefs(),
      nextPrefs() { }

ContouringCommandImplementation::~ContouringCommandImplementation() { }


ContouringCommand::ContouringCommand(QUndoCommand *parent)
    : QUndoCommand(parent),
      m(new ContouringCommandImplementation) { }


void ContouringCommand::init(HeightMapLogic *l)
{
    m->logic = l;
    m->prevPrefs = l->preferences();
}

HeightMapLogic *ContouringCommand::logic() const
{ return m->logic; }

Preferences ContouringCommand::preferences() const
{ return m->nextPrefs; }

void ContouringCommand::setPreferences(const Preferences &prefs)
{ m->nextPrefs = prefs; }


void ContouringCommand::undo()
{
    m->logic->setPreferences(m->prevPrefs);
    m->logic->plotIsobars();
}

void ContouringCommand::redo()
{
    m->logic->setPreferences(m->nextPrefs);
    m->logic->plotIsobars();
}


ContouringCommand::~ContouringCommand()
{
    delete m;
}


} // namespace HeightMap
