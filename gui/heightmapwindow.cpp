#include <QLabel>
#include <QScrollArea>
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QImage>
#include <QPixmap>
#include <QThread>
#include "mappingworker.h"
#include "landscape.h"
#include "peakgenerationoptions.h"
#include "mappingdata.h"
#include "engraver.h"
#include "preferences.h"
#include "heightmapsettingsdialog.h"
#include "heightmapapplication.h"

#include "heightmapwindow.h"


namespace HeightMap {


struct HeightMapWindowImplementation
{
    HeightMapWindowImplementation();
    ~HeightMapWindowImplementation();

    QLabel *hmImgLabel;
    QLabel *stateLabel;

    PeakGenerationOptions genOptions;
    std::vector<PeakInfo> peaks;
    std::vector<int> levels;
    std::list<Line2dSegment> contours;
    Landscape landscape;

    Engraver engr;

    QThread procThread;

    bool processing;

private:
    DISABLE_COPY(HeightMapWindowImplementation)
    DISABLE_MOVE(HeightMapWindowImplementation)
};

HeightMapWindowImplementation::HeightMapWindowImplementation()
    : hmImgLabel(new QLabel),
      stateLabel(new QLabel),
      genOptions(),
      peaks(),
      levels(),
      contours(),
      landscape(1024, 1024),
      engr(),
      procThread(),
      processing(false)
{
    genOptions.hmHeight = 1024;
    genOptions.hmWidth = 1024;
    genOptions.minPeak = 100;
    genOptions.maxPeak = 200;
    genOptions.peakCount = 4096;

    for (int i = genOptions.minPeak; i <= genOptions.maxPeak; ++i)
        levels.push_back(i);
}

HeightMapWindowImplementation::~HeightMapWindowImplementation()
{
    procThread.quit();
    procThread.wait();
}


HeightMapWindow::HeightMapWindow(QWidget *parent)
    : QMainWindow(parent),
      m(new HeightMapWindowImplementation)
{
    adjustPreferences();

    MappingData hmData;
    hmData.genOptions = &m->genOptions;
    hmData.peaks = &m->peaks;
    hmData.levels = &m->levels;
    hmData.contours = &m->contours;
    hmData.landscape = &m->landscape;

    MappingWorker *worker = new MappingWorker;
    worker->initFrom(&hmData);
    worker->moveToThread(&m->procThread);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setBackgroundRole(QPalette::Light);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(m->hmImgLabel);
    setCentralWidget(scrollArea);

    QAction *actGenLs = new QAction(this);
    actGenLs->setText(tr("Generate"));
    actGenLs->setShortcut(tr("Ctrl+G"));
    connect(actGenLs, SIGNAL(triggered()), worker, SLOT(createLandscape()));

    QAction *actHmSettings = new QAction(this);
    actHmSettings->setText(tr("Settings..."));
    connect(actHmSettings, SIGNAL(triggered()), this, SLOT(editHeightMapSettings()));

    QMenu *mnuLandscape = menuBar()->addMenu(tr("Landscape"));
    mnuLandscape->addAction(actGenLs);
    mnuLandscape->addAction(actHmSettings);

    statusBar()->addWidget(m->stateLabel, 1);

    connect(hmApp, SIGNAL(preferencesChanged()), this, SLOT(adjustPreferences()));

    connect(worker, SIGNAL(processStarted()), this, SLOT(onProcessStarted()));
    connect(worker, SIGNAL(processFinished()), this, SLOT(onProcessFinished()));

    connect(&m->procThread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    m->procThread.start();
}

HeightMapWindow::~HeightMapWindow()
{
    delete m;
}


void HeightMapWindow::editHeightMapSettings()
{
    HeightMapSettingsDialog dialog(this);
    dialog.setPreferences(hmApp->preferences());

    if (dialog.exec()) {
        hmApp->setPreferences(dialog.preferences());
    }
}

void HeightMapWindow::adjustPreferences()
{
    Preferences prefs = hmApp->preferences();
    m->genOptions.hmHeight = prefs.landscapeHeight();
    m->genOptions.hmWidth = prefs.landscapeWidth();

    m->genOptions.minPeak = prefs.minPeak();
    m->genOptions.maxPeak = prefs.maxPeak();
    m->genOptions.peakCount = prefs.peakCount();
}

void HeightMapWindow::onProcessStarted()
{
    m->processing = true;
    m->stateLabel->setText(tr("Processing..."));
}

void HeightMapWindow::onProcessFinished()
{
    m->processing = false;
    m->stateLabel->setText(tr("Done"));

    const int ImageFactor = 4;
    QImage hmImg(m->genOptions.hmWidth * ImageFactor,
                 m->genOptions.hmWidth * ImageFactor,
                 QImage::Format_ARGB32_Premultiplied);
    m->engr.drawIsobars(m->contours, &hmImg, true, ImageFactor);

    m->hmImgLabel->setPixmap(QPixmap::fromImage(hmImg));
}


} // namespace HeightMap
