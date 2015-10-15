#ifndef _PeakOptionsWidget_h_
#define _PeakOptionsWidget_h_


#include <QWidget>
#include "hmdef.h"
#include "../ge_shared.h"


namespace HeightMap {


struct PeakOptionsWidgetImplementation;
class HMGE_SHARED PeakOptionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PeakOptionsWidget(QWidget *parent = 0);

    int minPeak() const;
    int maxPeak() const;
    void setRange(int minValue, int maxValue);

    unsigned int peakCount() const;
    void setPeakCount(unsigned int);

    ~PeakOptionsWidget();

signals:
    void minPeakChanged(int);
    void maxPeakChanged(int);
    void peakCountChanged(int);

private:
    DISABLE_COPY(PeakOptionsWidget)
    DISABLE_MOVE(PeakOptionsWidget)

    PeakOptionsWidgetImplementation *m;
};


} // namespace HeightMap


#endif // _PeakOptionsWidget_h_
