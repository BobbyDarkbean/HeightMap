#ifndef _ExtrapolateCommand_h_
#define _ExtrapolateCommand_h_


#include <QUndoCommand>
#include "hmdef.h"


namespace HeightMap {


class HeightMapLogic;
class Preferences;
class ExtrapolationData;
struct ExtrapolateCommandImplementation;
class ExtrapolateCommand : public QUndoCommand
{
public:
    explicit ExtrapolateCommand(QUndoCommand *parent = 0);

    void init(HeightMapLogic *);

    HeightMapLogic *logic() const;

    Preferences preferences() const;
    void setPreferences(const Preferences &);

    ExtrapolationData xData() const;
    void setXData(const ExtrapolationData &);

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
