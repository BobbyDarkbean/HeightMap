#ifndef _ContouringOptionsDialog_h_
#define _ContouringOptionsDialog_h_


#include <QDialog>
#include "hm_shared.h"


namespace HeightMap {


class Preferences;
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

    const Preferences &preferences() const;
    void setPreferences(const Preferences &);

    ~ContouringOptionsDialog();

public slots:
    void done(int);

private:
    DISABLE_COPY(ContouringOptionsDialog)
    DISABLE_MOVE(ContouringOptionsDialog)

    ContouringOptionsDialogImplementation *m;
};


} // namespace HeightMap


#endif // _ContouringOptionsDialog_h_
