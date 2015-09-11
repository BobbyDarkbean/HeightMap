#ifndef _MappingWorker_h_
#define _MappingWorker_h_


#include <QObject>
#include <QLine>
#include "event.h"
#include "line2dsegment.h"
#include "peakinfo.h"


namespace HeightMap {


class Mapper;
class MappingWorker : public QObject
{
    Q_OBJECT

public:
    explicit MappingWorker(QObject *parent = 0);
    ~MappingWorker();

signals:
    void peakExtrapolated(QPoint, double);
    void contouringAt(QLine, int);

public slots:
    void generatePeaks();
    void extrapolatePeaks();
    void calculateContours();

private:
    Q_DISABLE_COPY(MappingWorker)

declare_eventhandler(MappingWorker, Mapper, peakExtrapolated, PeakInfo)
declare_eventhandler(MappingWorker, Mapper, contouringAt, Line2dSegment)
};


} // namespace HeightMap


#endif // _MappingWorker_h_
