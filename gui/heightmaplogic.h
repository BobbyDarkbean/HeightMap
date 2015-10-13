#ifndef _HeightMapLogic_h_
#define _HeightMapLogic_h_


#include <QObject>
#include <QPoint>
#include "hm_shared.h"
#include "heightmapviewmode.h"


class QImage;
namespace HeightMap {


class Terrain;
class Preferences;
class PreferencesController;
class ExtrapolationData;
class ExtrapolationFactory;
struct HeightMapLogicImplementation;
class HeightMapLogic : public QObject
{
    Q_OBJECT

public:
    explicit HeightMapLogic(QObject *parent = 0);

    Terrain *terrain();
    const Terrain *terrain() const;

    const Preferences &preferences() const;
    void setPreferences(const Preferences &);

    ExtrapolationData xData() const;
    void setXData(const ExtrapolationData &);

    ExtrapolationFactory *currentExtrapolation() const;

    const QImage &heightMapImage(HeightMapViewMode);

    ~HeightMapLogic();

signals:
    void terrainCreated();
    void processStarted();
    void processFinished();
    void peakGeneratingStarted();
    void peakGeneratingFinished();
    void peakExtrapolationStarted();
    void peakExtrapolated(QPoint, double);
    void peakExtrapolationFinished();
    void contouringStarted();
    void contouringLevelsAcquired(int);
    void contouringAt(int);
    void contouringFinished();

public slots:
    void newTerrain();
    void loadTerrain();
    void createLandscape();
    void buildLandscapeFromPeaks();
    void plotIsobars();

private:
    DISABLE_COPY(HeightMapLogic)
    DISABLE_MOVE(HeightMapLogic)

    HeightMapLogicImplementation *m;
};


} // namespace HeightMap


#endif // _HeightMapLogic_h_
