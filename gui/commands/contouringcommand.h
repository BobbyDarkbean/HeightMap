#ifndef _ContouringCommand_h_
#define _ContouringCommand_h_


#include <QUndoCommand>
#include "hm_shared.h"


namespace HeightMap {


class HeightMapLogic;
class Preferences;
struct ContouringCommandImplementation;
class ContouringCommand : public QUndoCommand
{
public:
    explicit ContouringCommand(QUndoCommand *parent = 0);

    void init(HeightMapLogic *);

    HeightMapLogic *logic() const;

    Preferences preferences() const;
    void setPreferences(const Preferences &);

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
