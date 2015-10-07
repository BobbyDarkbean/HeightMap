#include "../trigger.h"
#include "generatecommand.h"


namespace HeightMap {


struct GenerateCommandImplementation
{
    GenerateCommandImplementation();
    ~GenerateCommandImplementation();

    HeightMapLogic *logic;
    Trigger *trg;

private:
    DISABLE_COPY(GenerateCommandImplementation)
    DISABLE_MOVE(GenerateCommandImplementation)
};


GenerateCommandImplementation::GenerateCommandImplementation()
    : logic(nullptr),
      trg(nullptr) { }

GenerateCommandImplementation::~GenerateCommandImplementation() { }


GenerateCommand::GenerateCommand(QUndoCommand *parent)
    : QUndoCommand(parent),
      m(new GenerateCommandImplementation) { }


HeightMapLogic *GenerateCommand::logic() const
{ return m->logic; }

void GenerateCommand::setLogic(HeightMapLogic *l)
{ m->logic = l; }

Trigger *GenerateCommand::trigger() const
{ return m->trg; }

void GenerateCommand::setTrigger(Trigger *trg)
{ m->trg = trg; }


void GenerateCommand::undo()
{
    m->trg->activate();
}

void GenerateCommand::redo()
{
    m->trg->activate();
}


GenerateCommand::~GenerateCommand()
{
    delete m;
}


} // namespace HeightMap
