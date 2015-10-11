#ifndef _GenerateCommand_h_
#define _GenerateCommand_h_


#include <QUndoCommand>
#include "hm_shared.h"


namespace HeightMap {


class HeightMapLogic;
class Preferences;
class ExtrapolationData;
class Trigger;
struct GenerateCommandImplementation;
class GenerateCommand : public QUndoCommand
{
public:
    explicit GenerateCommand(QUndoCommand *parent = 0);

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

    ~GenerateCommand();

private:
    DISABLE_COPY(GenerateCommand)
    DISABLE_MOVE(GenerateCommand)

    GenerateCommandImplementation *m;
};


} // namespace HeightMap


#endif // _GenerateCommand_h_
