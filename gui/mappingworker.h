#ifndef _MappingWorker_h_
#define _MappingWorker_h_


#include <QObject>
#include <QLine>
#include "eventservice"
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

    void _emit_peakExtrapolated(QPoint, double);
    void _emit_contouringAt(QLine, int);

declare_eventhandler(MappingWorker, Mapper, peakExtrapolated, PeakInfo)
declare_eventhandler(MappingWorker, Mapper, contouringAt, Line2dSegment)
};


inline void MappingWorker::_emit_peakExtrapolated(QPoint coords, double height)
{ emit peakExtrapolated(coords, height); }

inline void MappingWorker::_emit_contouringAt(QLine segment, int level)
{ emit contouringAt(segment, level); }


} // namespace HeightMap


#endif // _MappingWorker_h_
