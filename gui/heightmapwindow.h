#ifndef _HeightMapWindow_h_
#define _HeightMapWindow_h_


#include <QMainWindow>
#include "hm_shared.h"


namespace HeightMap {


struct HeightMapWindowImplementation;
class HeightMapWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HeightMapWindow(QWidget *parent = 0);
    ~HeightMapWindow();

private slots:
    void exportLandscape();
    void exportPeaks();

    void editHeightMapSettings();

    void adjustPreferences();

    void onProcessStarted();
    void onProcessFinished();

    void onPeakGeneratingStarted();
    void onPeakGeneratingFinished();
    void onPeakExtrapolationStarted();
    void onPeakExtrapolated(QPoint, double);
    void onPeakExtrapolationFinished();
    void onContouringStarted();
    void onContouringAt(int);
    void onContouringFinished();

    void setViewMode(QAction *);

private:
    DISABLE_COPY(HeightMapWindow)
    DISABLE_MOVE(HeightMapWindow)

    HeightMapWindowImplementation *m;
};


} // namespace HeightMap


#endif // _HeightMapWindow_h_
