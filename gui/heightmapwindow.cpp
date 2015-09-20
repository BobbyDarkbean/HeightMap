#include <QLabel>
#include <QProgressBar>
#include <QScrollArea>
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QBoxLayout>
#include <QImage>
#include <QPixmap>
#include <QThread>
#include "mappingworker.h"
#include "landscape.h"
#include "peakgenerationoptions.h"
#include "mappingdata.h"
#include "preferences.h"
#include "heightmapsettingsdialog.h"
#include "heightmapapplication.h"

#include "heightmapwindow.h"


namespace HeightMap {


namespace {
enum HeightMapViewMode {
    HMVM_Landscape  = 0,
    HMVM_Isobars    = 1,
    HMVM_Hybrid     = 2
};
}


struct HeightMapWindowImplementation
{
    HeightMapWindowImplementation();

    void createActions(HeightMapWindow *, MappingWorker *);

    void provideMappingData(MappingWorker *);
    void displayHeightMapImage();

    ~HeightMapWindowImplementation();

    QLabel *hmImgLabel;

    QLabel *stateLabel;
    QLabel *lvlsLabel;
    QLabel *procLabel;
    QProgressBar *procBar;
    QLabel *pkLabel;
    QLabel *lsSizeLabel;
    QLabel *cntrsLabel;

    PeakGenerationOptions genOptions;
    std::vector<PeakInfo> peaks;
    std::vector<int> levels;
    std::list<Line2dSegment> contours;
    Landscape landscape;

    QImage imgLandscape;
    QImage imgIsobars;
    QImage imgHybrid;

    HeightMapViewMode hmvm;

    QThread procThread;

    bool processing;

private:
    DISABLE_COPY(HeightMapWindowImplementation)
    DISABLE_MOVE(HeightMapWindowImplementation)
};


HeightMapWindowImplementation::HeightMapWindowImplementation()
    : hmImgLabel(new QLabel),
      stateLabel(new QLabel),
      lvlsLabel(new QLabel),
      procLabel(new QLabel),
      procBar(new QProgressBar),
      pkLabel(new QLabel),
      lsSizeLabel(new QLabel),
      cntrsLabel(new QLabel),
      genOptions(),
      peaks(),
      levels(),
      contours(),
      landscape(Preferences::DefaultLandscapeWidth, Preferences::DefaultLandscapeHeight),
      imgLandscape(),
      imgIsobars(),
      imgHybrid(),
      hmvm(HMVM_Isobars),
      procThread(),
      processing(false) { }

void HeightMapWindowImplementation::createActions(HeightMapWindow *master, MappingWorker *worker)
{
    QAction *actExit = new QAction(master);
    actExit->setText(HeightMapWindow::tr("E&xit"));
    actExit->setShortcut(HeightMapWindow::tr("Alt+X"));
    QObject::connect(actExit, SIGNAL(triggered()), master, SLOT(close()));

    QMenu *mnuFile = master->menuBar()->addMenu(HeightMapWindow::tr("&File"));
    mnuFile->addAction(actExit);

    QAction *actGenLs = new QAction(master);
    actGenLs->setText(HeightMapWindow::tr("&Generate"));
    actGenLs->setShortcut(HeightMapWindow::tr("Ctrl+G"));
    QObject::connect(actGenLs, SIGNAL(triggered()), worker, SLOT(createLandscape()));

    QAction *actHmSettings = new QAction(master);
    actHmSettings->setText(HeightMapWindow::tr("&Settings..."));
    actHmSettings->setShortcut(HeightMapWindow::tr("Alt+F7"));
    QObject::connect(actHmSettings, SIGNAL(triggered()), master, SLOT(editHeightMapSettings()));

    QMenu *mnuLandscape = master->menuBar()->addMenu(HeightMapWindow::tr("&Landscape"));
    mnuLandscape->addAction(actGenLs);
    mnuLandscape->addSeparator();
    mnuLandscape->addAction(actHmSettings);

    QActionGroup *agpViewMode = new QActionGroup(master);
    QObject::connect(agpViewMode, SIGNAL(triggered(QAction *)), master, SLOT(setViewMode(QAction *)));

    QAction *actViewModeLandscape = new QAction(agpViewMode);
    actViewModeLandscape->setText(HeightMapWindow::tr("Landscape"));
    actViewModeLandscape->setCheckable(true);
    actViewModeLandscape->setProperty("hmvm", HMVM_Landscape);

    QAction *actViewModeIsobars = new QAction(agpViewMode);
    actViewModeIsobars->setText(HeightMapWindow::tr("Isobars"));
    actViewModeIsobars->setCheckable(true);
    actViewModeIsobars->setProperty("hmvm", HMVM_Isobars);

    QAction *actViewModeHybrid = new QAction(agpViewMode);
    actViewModeHybrid->setText(HeightMapWindow::tr("Hybrid"));
    actViewModeHybrid->setCheckable(true);
    actViewModeHybrid->setProperty("hmvm", HMVM_Hybrid);

    actViewModeIsobars->setChecked(true);

    QMenu *mnuView = master->menuBar()->addMenu(HeightMapWindow::tr("&View"));
    mnuView->addAction(actViewModeLandscape);
    mnuView->addAction(actViewModeIsobars);
    mnuView->addAction(actViewModeHybrid);
}

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

    worker->initFrom(hmData);
}

void HeightMapWindowImplementation::displayHeightMapImage()
{
    switch (hmvm) {
    case HMVM_Landscape:
        hmImgLabel->setPixmap(QPixmap::fromImage(imgLandscape));
        break;
    case HMVM_Isobars:
        hmImgLabel->setPixmap(QPixmap::fromImage(imgIsobars));
        break;
    case HMVM_Hybrid:
        hmImgLabel->setPixmap(QPixmap::fromImage(imgHybrid));
        break;
    default:
        break;
    }
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

    m->createActions(this, worker);

    m->hmImgLabel->setAlignment(Qt::AlignCenter);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(m->hmImgLabel);
    setCentralWidget(scrollArea);

    QWidget *procBarShell = new QWidget(this);
    QBoxLayout *procBarLayout = new QHBoxLayout(procBarShell);
    procBarLayout->setMargin(0);
    procBarLayout->addWidget(m->procBar);
    procBarShell->setMaximumHeight(m->procLabel->sizeHint().height());

    m->procBar->hide();

    statusBar()->addWidget(m->stateLabel, 6);
    statusBar()->addWidget(m->lsSizeLabel, 3);
    statusBar()->addWidget(m->lvlsLabel, 3);
    statusBar()->addWidget(m->procLabel, 8);
    statusBar()->addWidget(procBarShell, 12);
    statusBar()->addWidget(m->pkLabel, 4);
    statusBar()->addWidget(m->cntrsLabel, 6);

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

    m->levels.clear();
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
    m->lsSizeLabel->setText(QString("%1%2%3")
                            .arg(m->landscape.width())
                            .arg(QChar(0x00d7))
                            .arg(m->landscape.height()));
    m->lvlsLabel->setText(tr("%1 level(s)").arg(m->levels.size()));

    m->procBar->setValue(0);
    m->procBar->setMaximum(m->genOptions.peakCount + m->landscape.width() - 1);
    m->procBar->show();
    m->processing = true;
}

void HeightMapWindow::onProcessFinished()
{
    m->processing = false;
    m->stateLabel->setText(tr("Done"));
    m->procLabel->clear();
    m->procBar->hide();

    m->displayHeightMapImage();
}

void HeightMapWindow::onPeakGeneratingStarted()
{
    m->procLabel->setText("Generating peaks...");
}

void HeightMapWindow::onPeakGeneratingFinished()
{
    m->pkLabel->setText(tr("%1 peak(s)").arg(m->peaks.size()));
}

void HeightMapWindow::onPeakExtrapolationStarted()
{
    m->procLabel->setText("Extrapolating peaks...");
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
}

void HeightMapWindow::onContouringAt(int)
{
    m->procBar->setValue(m->procBar->value() + 1);
}

void HeightMapWindow::onContouringFinished()
{
    m->cntrsLabel->setText(tr("%1 isobar segment(s)").arg(m->contours.size()));
}

void HeightMapWindow::onOffScreenDrawingStarted()
{
    m->procLabel->setText("Drawing an off-screen representation...");
}

void HeightMapWindow::onOffScreenDrawingFinished()
{
}

void HeightMapWindow::setViewMode(QAction *viewModeAct)
{
    m->hmvm = static_cast<HeightMapViewMode>(viewModeAct->property("hmvm").toInt());
    m->displayHeightMapImage();
}


} // namespace HeightMap
