#ifndef _GenerateCommand_h_
#define _GenerateCommand_h_


#include <QUndoCommand>
#include "hm_shared.h"


namespace HeightMap {


class HeightMapLogic;
class Preferences;
class ExtrapolationData;
struct GenerateCommandImplementation;
class GenerateCommand : public QUndoCommand
{
public:
    explicit GenerateCommand(QUndoCommand *parent = 0);

    void init(HeightMapLogic *);

    HeightMapLogic *logic() const;

    Preferences preferences() const;
    void setPreferences(const Preferences &);

    ExtrapolationData xData() const;
    void setXData(const ExtrapolationData &);

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
