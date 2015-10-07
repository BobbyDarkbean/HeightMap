#include "../trigger.h"
#include "contouringcommand.h"


namespace HeightMap {


struct ContouringCommandImplementation
{
    ContouringCommandImplementation();
    ~ContouringCommandImplementation();

    HeightMapLogic *logic;
    Trigger *trg;

private:
    DISABLE_COPY(ContouringCommandImplementation)
    DISABLE_MOVE(ContouringCommandImplementation)
};


ContouringCommandImplementation::ContouringCommandImplementation()
    : logic(nullptr),
      trg(nullptr) { }

ContouringCommandImplementation::~ContouringCommandImplementation() { }


ContouringCommand::ContouringCommand(QUndoCommand *parent)
    : QUndoCommand(parent),
      m(new ContouringCommandImplementation) { }


HeightMapLogic *ContouringCommand::logic() const
{ return m->logic; }

void ContouringCommand::setLogic(HeightMapLogic *l)
{ m->logic = l; }

Trigger *ContouringCommand::trigger() const
{ return m->trg; }

void ContouringCommand::setTrigger(Trigger *trg)
{ m->trg = trg; }


void ContouringCommand::undo()
{
    m->trg->activate();
}

void ContouringCommand::redo()
{
    m->trg->activate();
}


ContouringCommand::~ContouringCommand()
{
    delete m;
}


} // namespace HeightMap
