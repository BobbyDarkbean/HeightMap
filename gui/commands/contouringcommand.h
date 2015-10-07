#ifndef _ContouringCommand_h_
#define _ContouringCommand_h_


#include <QUndoCommand>
#include "hm_shared.h"


namespace HeightMap {


class HeightMapLogic;
class Trigger;
struct ContouringCommandImplementation;
class ContouringCommand : public QUndoCommand
{
public:
    explicit ContouringCommand(QUndoCommand *parent = 0);

    HeightMapLogic *logic() const;
    void setLogic(HeightMapLogic *);

    Trigger *trigger() const;
    void setTrigger(Trigger *);

    void undo();
    void redo();

    ~ContouringCommand();

private:
    DISABLE_COPY(ContouringCommand)
    DISABLE_MOVE(ContouringCommand)

    ContouringCommandImplementation *m;
};


} // namespace HeightMap


#endif // _ContouringCommand_h_
