#ifndef _ExtrapolateCommand_h_
#define _ExtrapolateCommand_h_


#include <QUndoCommand>
#include "hm_shared.h"


namespace HeightMap {


class HeightMapLogic;
class Trigger;
struct ExtrapolateCommandImplementation;
class ExtrapolateCommand : public QUndoCommand
{
public:
    explicit ExtrapolateCommand(QUndoCommand *parent = 0);

    HeightMapLogic *logic() const;
    void setLogic(HeightMapLogic *);

    Trigger *trigger() const;
    void setTrigger(Trigger *);

    void undo();
    void redo();

    ~ExtrapolateCommand();

private:
    DISABLE_COPY(ExtrapolateCommand)
    DISABLE_MOVE(ExtrapolateCommand)

    ExtrapolateCommandImplementation *m;
};


} // namespace HeightMap


#endif // _ExtrapolateCommand_h_
