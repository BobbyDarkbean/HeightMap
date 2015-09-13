#ifndef _MappingWorker_h_
#define _MappingWorker_h_


#include <QObject>
#include <QPoint>
#include "eventservice"
#include "peakinfo.h"


namespace HeightMap {


class Mapper;
struct MappingData;
struct MappingWorkerImplementation;
class MappingWorker : public QObject
{
    Q_OBJECT

public:
    explicit MappingWorker(QObject *parent = 0);
    void initFrom(MappingData *);
    ~MappingWorker();

signals:
    void peakGeneratingFinished();
    void peakExtrapolated(QPoint, double);
    void peakExtrapolationFinished();
    void contouringAt(QPoint);
    void contouringFinished();

public slots:
    void generatePeaks();
    void extrapolatePeaks();
    void calculateContours();

private:
    DISABLE_COPY(MappingWorker)
    DISABLE_MOVE(MappingWorker)

    void _emit_peakExtrapolated(QPoint, double);
    void _emit_contouringAt(QPoint);

    MappingWorkerImplementation *m;

declare_eventhandler(MappingWorker, Mapper, peakExtrapolated, PeakInfo)
declare_eventhandler(MappingWorker, Mapper, contouringAt, PeakInfo)
};


inline void MappingWorker::_emit_peakExtrapolated(QPoint coords, double height)
{ emit peakExtrapolated(coords, height); }

inline void MappingWorker::_emit_contouringAt(QPoint coords)
{ emit contouringAt(coords); }


} // namespace HeightMap


#endif // _MappingWorker_h_
