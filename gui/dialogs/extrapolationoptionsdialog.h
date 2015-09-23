#ifndef _ExtrapolationOptionsDialog_h_
#define _ExtrapolationOptionsDialog_h_


#include <QDialog>
#include "hm_shared.h"


namespace HeightMap {


struct ExtrapolationOptionsDialogImplementation;
class ExtrapolationOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExtrapolationOptionsDialog(QWidget *parent = 0);

    int baseLevel() const;
    void setBaseLevel(int);

    ~ExtrapolationOptionsDialog();

private:
    DISABLE_COPY(ExtrapolationOptionsDialog)
    DISABLE_MOVE(ExtrapolationOptionsDialog)

    ExtrapolationOptionsDialogImplementation *m;
};


} // namespace HeightMap


#endif // _ExtrapolationOptionsDialog_h_
