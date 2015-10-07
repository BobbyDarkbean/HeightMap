#ifndef _GenerateCommand_h_
#define _GenerateCommand_h_


#include <QUndoCommand>
#include "hm_shared.h"


namespace HeightMap {


class HeightMapLogic;
class Trigger;
struct GenerateCommandImplementation;
class GenerateCommand : public QUndoCommand
{
public:
    explicit GenerateCommand(QUndoCommand *parent = 0);

    HeightMapLogic *logic() const;
    void setLogic(HeightMapLogic *);

    Trigger *trigger() const;
    void setTrigger(Trigger *);

    void undo();
    void redo();

    ~GenerateCommand();

private:
    DISABLE_COPY(GenerateCommand)
    DISABLE_MOVE(GenerateCommand)

    GenerateCommandImplementation *m;
};


} // namespace HeightMap


#endif // _GenerateCommand_h_
