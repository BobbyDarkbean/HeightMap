#ifndef _HeightMapWindow_h_
#define _HeightMapWindow_h_


#include <QMainWindow>
#include "hm_shared.h"


namespace HeightMap {


class HeightMapLogic;
struct HeightMapWindowImplementation;
class HeightMapWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HeightMapWindow(QWidget *parent = 0);
    void init(HeightMapLogic *);
    ~HeightMapWindow();

public slots:
    void newFile();

    void exportLandscape();
    void exportPeaks();

    void editPeakSettings();
    void editExtrapolationSettings();
    void editContouringSettings();

    void generateLandscape();
    void extrapolatePeaks();
    void calculateContours();

    void adjustPreferences();
    void adjustExtrapolationData(QString);

    void resetTerrainData();
    void onProcessStarted();
    void onProcessFinished();
    void onPeakGeneratingStarted();
    void onPeakGeneratingFinished();
    void onPeakExtrapolationStarted();
    void onPeakExtrapolated(QPoint, double);
    void onPeakExtrapolationFinished();
    void onContouringStarted();
    void onContouringLevelsAcquired(int);
    void onContouringAt(int);
    void onContouringFinished();

    void setViewMode(QAction *);

private:
    DISABLE_COPY(HeightMapWindow)
    DISABLE_MOVE(HeightMapWindow)

    void createCentral();
    void createWidgets();
    void createDocks();
    void createActions();
    void createStatusBar();

    HeightMapWindowImplementation *m;
};


} // namespace HeightMap


#endif // _HeightMapWindow_h_
