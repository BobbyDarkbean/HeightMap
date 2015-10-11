#ifndef _ExtrapolateCommand_h_
#define _ExtrapolateCommand_h_


#include <QUndoCommand>
#include "hm_shared.h"


namespace HeightMap {


class HeightMapLogic;
class Preferences;
class ExtrapolationData;
class Trigger;
struct ExtrapolateCommandImplementation;
class ExtrapolateCommand : public QUndoCommand
{
public:
    explicit ExtrapolateCommand(QUndoCommand *parent = 0);

    HeightMapLogic *logic() const;
    void setLogic(HeightMapLogic *);

    Preferences prevPreferences() const;
    void setPrevPreferences(const Preferences &);

    Preferences nextPreferences() const;
    void setNextPreferences(const Preferences &);

    ExtrapolationData prevXData() const;
    void setPrevXData(const ExtrapolationData &);

    ExtrapolationData nextXData() const;
    void setNextXData(const ExtrapolationData &);

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
