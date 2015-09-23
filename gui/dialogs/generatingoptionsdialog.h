#ifndef _GeneratingOptionsDialog_h_
#define _GeneratingOptionsDialog_h_


#include <QDialog>
#include "hm_shared.h"


namespace HeightMap {


struct GeneratingOptionsDialogImplementation;
class GeneratingOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GeneratingOptionsDialog(QWidget *parent = 0);

    int minPeak() const;
    int maxPeak() const;
    void setRange(int minValue, int maxValue);

    int peakCount() const;
    void setPeakCount(int);

    ~GeneratingOptionsDialog();

private:
    DISABLE_COPY(GeneratingOptionsDialog)
    DISABLE_MOVE(GeneratingOptionsDialog)

    GeneratingOptionsDialogImplementation *m;
};


} // namespace HeightMap


#endif // _GeneratingOptionsDialog_h_
