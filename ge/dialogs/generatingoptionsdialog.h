#ifndef _GeneratingOptionsDialog_h_
#define _GeneratingOptionsDialog_h_


#include <QDialog>
#include "hmdef.h"
#include "../ge_shared.h"


namespace HeightMap {


class PreferencesController;
struct GeneratingOptionsDialogImplementation;
class HMGE_SHARED GeneratingOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GeneratingOptionsDialog(QWidget *parent = 0);

    QSize sizeHint() const;

    int minPeak() const;
    int maxPeak() const;
    void setRange(int minValue, int maxValue);

    unsigned int peakCount() const;
    void setPeakCount(unsigned int);

    PreferencesController *preferencesController() const;
    void setPreferencesController(PreferencesController *);

    ~GeneratingOptionsDialog();

private:
    DISABLE_COPY(GeneratingOptionsDialog)
    DISABLE_MOVE(GeneratingOptionsDialog)

    GeneratingOptionsDialogImplementation *m;
};


} // namespace HeightMap


#endif // _GeneratingOptionsDialog_h_
