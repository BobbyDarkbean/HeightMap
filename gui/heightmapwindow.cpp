#include <QLabel>
#include <QProgressBar>
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
    void provideMappingData(MappingWorker *);
    ~HeightMapWindowImplementation();

    QLabel *hmImgLabel;

    QLabel *stateLabel;
    QLabel *procLabel;
    QProgressBar *procBar;

    PeakGenerationOptions genOptions;
    std::vector<PeakInfo> peaks;
    std::vector<int> levels;
    std::list<Line2dSegment> contours;
    Landscape landscape;

    QImage imgLandscape;
    QImage imgIsobars;
    QImage imgHybrid;

    QThread procThread;

    bool processing;

private:
    DISABLE_COPY(HeightMapWindowImplementation)
    DISABLE_MOVE(HeightMapWindowImplementation)
};

HeightMapWindowImplementation::HeightMapWindowImplementation()
    : hmImgLabel(new QLabel),
      stateLabel(new QLabel),
      procLabel(new QLabel),
      procBar(new QProgressBar),
      genOptions(),
      peaks(),
      levels(),
      contours(),
      landscape(Preferences::DefaultLandscapeWidth, Preferences::DefaultLandscapeHeight),
      imgLandscape(),
      imgIsobars(),
      imgHybrid(),
      procThread(),
      processing(false) { }

void HeightMapWindowImplementation::provideMappingData(MappingWorker *worker)
{
    MappingData hmData = {
        &genOptions,
        &peaks,
        &levels,
        &contours,
        &landscape,

        &imgLandscape,
        &imgIsobars,
        &imgHybrid
    };

    worker->initFrom(&hmData);
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

    MappingWorker *worker = new MappingWorker;
    m->provideMappingData(worker);
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
    statusBar()->addWidget(m->procLabel, 1);
    statusBar()->addWidget(m->procBar, 2);

    connect(hmApp, SIGNAL(preferencesChanged()), this, SLOT(adjustPreferences()));

    connect(worker, SIGNAL(processStarted()), this, SLOT(onProcessStarted()), Qt::BlockingQueuedConnection);
    connect(worker, SIGNAL(processFinished()), this, SLOT(onProcessFinished()), Qt::BlockingQueuedConnection);

    connect(worker, SIGNAL(peakGeneratingStarted()), this, SLOT(onPeakGeneratingStarted()), Qt::BlockingQueuedConnection);
    connect(worker, SIGNAL(peakGeneratingFinished()), this, SLOT(onPeakGeneratingFinished()), Qt::BlockingQueuedConnection);
    connect(worker, SIGNAL(peakExtrapolationStarted()), this, SLOT(onPeakExtrapolationStarted()), Qt::BlockingQueuedConnection);
    connect(worker, SIGNAL(peakExtrapolated(QPoint, double)), this, SLOT(onPeakExtrapolated(QPoint, double)));
    connect(worker, SIGNAL(peakExtrapolationFinished()), this, SLOT(onPeakExtrapolationFinished()), Qt::BlockingQueuedConnection);
    connect(worker, SIGNAL(contouringStarted()), this, SLOT(onContouringStarted()), Qt::BlockingQueuedConnection);
    connect(worker, SIGNAL(contouringAt(int)), this, SLOT(onContouringAt(int)));
    connect(worker, SIGNAL(contouringFinished()), this, SLOT(onContouringFinished()), Qt::BlockingQueuedConnection);
    connect(worker, SIGNAL(offScreenDrawingStarted()), this, SLOT(onOffScreenDrawingStarted()), Qt::BlockingQueuedConnection);
    connect(worker, SIGNAL(offScreenDrawingFinished()), this, SLOT(onOffScreenDrawingFinished()), Qt::BlockingQueuedConnection);

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

    for (int i = m->genOptions.minPeak; i <= m->genOptions.maxPeak; ++i)
        m->levels.push_back(i);
}

void HeightMapWindow::onProcessStarted()
{
    m->peaks.clear();
    m->contours.clear();
    m->landscape = Landscape(m->genOptions.hmWidth,
                             m->genOptions.hmHeight);

    m->stateLabel->setText(tr("Processing..."));
    m->procBar->show();
    m->processing = true;
}

void HeightMapWindow::onProcessFinished()
{
    m->processing = false;
    m->stateLabel->setText(tr("Done"));
    m->procLabel->clear();
    m->procBar->hide();

    m->hmImgLabel->setPixmap(QPixmap::fromImage(m->imgHybrid));
}

void HeightMapWindow::onPeakGeneratingStarted()
{
    m->procLabel->setText("Generating peaks...");
    m->procBar->setValue(0);
    m->procBar->setMaximum(m->genOptions.peakCount);
}

void HeightMapWindow::onPeakGeneratingFinished()
{
}

void HeightMapWindow::onPeakExtrapolationStarted()
{
    m->procLabel->setText("Extrapolating peaks...");
    m->procBar->setValue(0);
    m->procBar->setMaximum(m->genOptions.peakCount);
}

void HeightMapWindow::onPeakExtrapolated(QPoint, double)
{
    m->procBar->setValue(m->procBar->value() + 1);
}

void HeightMapWindow::onPeakExtrapolationFinished()
{
}

void HeightMapWindow::onContouringStarted()
{
    m->procLabel->setText("Calculating contours...");
    m->procBar->setValue(0);
    m->procBar->setMaximum(m->landscape.width());
}

void HeightMapWindow::onContouringAt(int)
{
    m->procBar->setValue(m->procBar->value() + 1);
}

void HeightMapWindow::onContouringFinished()
{
}

void HeightMapWindow::onOffScreenDrawingStarted()
{
    m->procLabel->setText("Drawing an off-screen representation...");
}

void HeightMapWindow::onOffScreenDrawingFinished()
{
}


} // namespace HeightMap
