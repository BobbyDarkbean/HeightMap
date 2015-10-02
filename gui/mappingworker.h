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
    void initFrom(const MappingData &);
    ~MappingWorker();

signals:
    void processStarted();
    void processFinished();
    void peakGeneratingStarted();
    void peakGeneratingFinished();
    void peakExtrapolationStarted();
    void peakExtrapolationAcquiring();
    void peakExtrapolated(QPoint, double);
    void peakExtrapolationFinished();
    void contouringStarted();
    void contouringLevelsAcquired(int);
    void contouringAt(int);
    void contouringFinished();

public slots:
    void createLandscape();
    void buildLandscapeFromPeaks();
    void plotIsobars();

private:
    DISABLE_COPY(MappingWorker)
    DISABLE_MOVE(MappingWorker)

    void generatePeaks();
    void extrapolatePeaks();
    void calculateContours();

    void _emit_peakExtrapolated(QPoint, double);
    void _emit_contouringAt(int);

    MappingWorkerImplementation *m;

declare_eventhandler(MappingWorker, Mapper, peakExtrapolated, PeakInfo)
declare_eventhandler(MappingWorker, Mapper, contouringAt, int)
};


inline void MappingWorker::_emit_peakExtrapolated(QPoint coords, double height)
{ emit peakExtrapolated(coords, height); }

inline void MappingWorker::_emit_contouringAt(int x)
{ emit contouringAt(x); }


} // namespace HeightMap


#endif // _MappingWorker_h_
