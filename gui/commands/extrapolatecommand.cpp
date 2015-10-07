#include "../trigger.h"
#include "extrapolatecommand.h"


namespace HeightMap {


struct ExtrapolateCommandImplementation
{
    ExtrapolateCommandImplementation();
    ~ExtrapolateCommandImplementation();

    HeightMapLogic *logic;
    Trigger *trg;

private:
    DISABLE_COPY(ExtrapolateCommandImplementation)
    DISABLE_MOVE(ExtrapolateCommandImplementation)
};


ExtrapolateCommandImplementation::ExtrapolateCommandImplementation()
    : logic(nullptr),
      trg(nullptr) { }

ExtrapolateCommandImplementation::~ExtrapolateCommandImplementation() { }


ExtrapolateCommand::ExtrapolateCommand(QUndoCommand *parent)
    : QUndoCommand(parent),
      m(new ExtrapolateCommandImplementation) { }


HeightMapLogic *ExtrapolateCommand::logic() const
{ return m->logic; }

void ExtrapolateCommand::setLogic(HeightMapLogic *l)
{ m->logic = l; }

Trigger *ExtrapolateCommand::trigger() const
{ return m->trg; }

void ExtrapolateCommand::setTrigger(Trigger *trg)
{ m->trg = trg; }


void ExtrapolateCommand::undo()
{
    m->trg->activate();
}

void ExtrapolateCommand::redo()
{
    m->trg->activate();
}


ExtrapolateCommand::~ExtrapolateCommand()
{
    delete m;
}


} // namespace HeightMap
