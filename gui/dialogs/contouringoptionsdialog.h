#ifndef _ContouringOptionsDialog_h_
#define _ContouringOptionsDialog_h_


#include <QDialog>
#include "hm_shared.h"


namespace HeightMap {


class PreferencesController;
struct ContouringOptionsDialogImplementation;
class ContouringOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ContouringOptionsDialog(QWidget *parent = 0);

    int minLevel() const;
    int maxLevel() const;
    void setLevelRange(int minValue, int maxValue);

    int step() const;
    void setStep(int);

    PreferencesController *preferencesController() const;
    void setPreferencesController(PreferencesController *);

    ~ContouringOptionsDialog();

private:
    DISABLE_COPY(ContouringOptionsDialog)
    DISABLE_MOVE(ContouringOptionsDialog)

    ContouringOptionsDialogImplementation *m;
};


} // namespace HeightMap


#endif // _ContouringOptionsDialog_h_
