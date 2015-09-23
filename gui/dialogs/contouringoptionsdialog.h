#ifndef _ContouringOptionsDialog_h_
#define _ContouringOptionsDialog_h_


#include <QDialog>
#include "hm_shared.h"


namespace HeightMap {


struct ContouringOptionsDialogImplementation;
class ContouringOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ContouringOptionsDialog(QWidget *parent = 0);

    int minLevel() const;
    void setMinLevel(int);

    int maxLevel() const;
    void setMaxLevel(int);

    int step() const;
    void setStep(int);

    ~ContouringOptionsDialog();

private:
    DISABLE_COPY(ContouringOptionsDialog)
    DISABLE_MOVE(ContouringOptionsDialog)

    ContouringOptionsDialogImplementation *m;
};


} // namespace HeightMap


#endif // _ContouringOptionsDialog_h_
