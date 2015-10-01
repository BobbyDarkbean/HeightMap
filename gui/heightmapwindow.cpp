#include <QLabel>
#include <QProgressBar>
#include <QScrollArea>
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QBoxLayout>
#include <QImage>
#include <QPixmap>
#include <QFileDialog>
#include <fstream>
#include "auxiliary/mappingdata.h"
#include "heightmapapplication.h"
#include "mappingthread.h"
#include "mappingworker.h"
#include "preferences.h"
#include "preferencescontroller.h"
#include "terrain.h"
#include "widgets/abstractextrapolationwidget.h"
#include "widgets/peakoptionswidget.h"
#include "widgets/extrapolationoptionswidget.h"
#include "widgets/contouringoptionswidget.h"
#include "dialogs/terrainoptionsdialog.h"
#include "dialogs/generatingoptionsdialog.h"
#include "dialogs/extrapolationoptionsdialog.h"
#include "dialogs/contouringoptionsdialog.h"

#include "heightmapwindow.h"


namespace HeightMap {


namespace {
enum HeightMapViewMode {
    HMVM_Peaks      = 0,
    HMVM_Landscape  = 1,
    HMVM_Isobars    = 2,
    HMVM_Hybrid     = 3
};

const char *LS_TEXT_FILE_EXT = "hmlst";
const char *PK_TEXT_FILE_EXT = "hmpkt";
}


struct HeightMapWindowImplementation
{
    HeightMapWindowImplementation();

    void createActions(HeightMapWindow *, MappingWorker *);
    void createWidgets();
    void createDocks(HeightMapWindow *master);

    void provideMappingData(MappingWorker *);
    void provideExtrapolationWidgets(ExtrapolationOptionsDialog *);

    void displayHeightMapImage();
    void resetImages();
    void resetStatusBar();

    ~HeightMapWindowImplementation();

    QLabel *hmImgLabel;

    PeakOptionsWidget *wgtPeakGenerating;
    ExtrapolationOptionsWidget *wgtExtrapolation;
    ContouringOptionsWidget *wgtContouring;

    QDockWidget *dckPeakGenerating;
    QDockWidget *dckExtrapolation;
    QDockWidget *dckContouring;

    QLabel *stateLabel;
    QLabel *lvlsLabel;
    QLabel *procLabel;
    QProgressBar *procBar;
    QLabel *pkLabel;
    QLabel *lsSizeLabel;
    QLabel *cntrsLabel;

    Terrain terrain;

    QImage imgPeaks;
    QImage imgLandscape;
    QImage imgIsobars;
    QImage imgHybrid;

    HeightMapViewMode hmvm;

    MappingThread procThread;

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
      terrain(hmApp->preferences().landscapeWidth(), hmApp->preferences().landscapeHeight()),
      imgPeaks(),
      imgLandscape(),
      imgIsobars(),
      imgHybrid(),
      hmvm(HMVM_Hybrid),
      procThread(),
      processing(false) { }

void HeightMapWindowImplementation::createActions(HeightMapWindow *master, MappingWorker *worker)
{
    QAction *actNewFile = new QAction(master);
    actNewFile->setText(HeightMapWindow::tr("&New file..."));
    actNewFile->setShortcut(HeightMapWindow::tr("Ctrl+N"));
    QObject::connect(actNewFile, SIGNAL(triggered()), master, SLOT(newFile()));

    QAction *actExportLs = new QAction(master);
    actExportLs->setText(HeightMapWindow::tr("Export landscape..."));
    QObject::connect(actExportLs, SIGNAL(triggered()), master, SLOT(exportLandscape()));

    QAction *actExportPk = new QAction(master);
    actExportPk->setText(HeightMapWindow::tr("Export peaks..."));
    QObject::connect(actExportPk, SIGNAL(triggered()), master, SLOT(exportPeaks()));

    QAction *actExit = new QAction(master);
    actExit->setText(HeightMapWindow::tr("E&xit"));
    actExit->setShortcut(HeightMapWindow::tr("Alt+X"));
    QObject::connect(actExit, SIGNAL(triggered()), master, SLOT(close()));

    QMenu *mnuFile = master->menuBar()->addMenu(HeightMapWindow::tr("&File"));
    mnuFile->addAction(actNewFile);
    mnuFile->addSeparator();
    mnuFile->addAction(actExportLs);
    mnuFile->addAction(actExportPk);
    mnuFile->addSeparator();
    mnuFile->addAction(actExit);

    QAction *actGenLs = new QAction(master);
    actGenLs->setText(HeightMapWindow::tr("&Generate landscape"));
    actGenLs->setShortcut(HeightMapWindow::tr("Ctrl+G"));
    QObject::connect(actGenLs, SIGNAL(triggered()), worker, SLOT(createLandscape()));

    QAction *actBuildLs = new QAction(master);
    actBuildLs->setText(HeightMapWindow::tr("E&xtrapolate peaks"));
    actBuildLs->setShortcut(HeightMapWindow::tr("Ctrl+E"));
    QObject::connect(actBuildLs, SIGNAL(triggered()), worker, SLOT(buildLandscapeFromPeaks()));

    QAction *actCalcContours = new QAction(master);
    actCalcContours->setText(HeightMapWindow::tr("Calculate &isobars"));
    actCalcContours->setShortcut(HeightMapWindow::tr("Ctrl+I"));
    QObject::connect(actCalcContours, SIGNAL(triggered()), worker, SLOT(plotIsobars()));

    QAction *actHmSettings = new QAction(master);
    actHmSettings->setText(HeightMapWindow::tr("&Peak settings..."));
    actHmSettings->setShortcut(HeightMapWindow::tr("F6"));
    QObject::connect(actHmSettings, SIGNAL(triggered()), master, SLOT(editPeakSettings()));

    QAction *actExtrapolSettings = new QAction(master);
    actExtrapolSettings->setText(HeightMapWindow::tr("&Extrapolation settings..."));
    actExtrapolSettings->setShortcut(HeightMapWindow::tr("F7"));
    QObject::connect(actExtrapolSettings, SIGNAL(triggered()), master, SLOT(editExtrapolationSettings()));

    QAction *actContourSettings = new QAction(master);
    actContourSettings->setText(HeightMapWindow::tr("&Contouring settings..."));
    actContourSettings->setShortcut(HeightMapWindow::tr("F8"));
    QObject::connect(actContourSettings, SIGNAL(triggered()), master, SLOT(editContouringSettings()));

    QMenu *mnuLandscape = master->menuBar()->addMenu(HeightMapWindow::tr("&Landscape"));
    mnuLandscape->addAction(actGenLs);
    mnuLandscape->addAction(actBuildLs);
    mnuLandscape->addAction(actCalcContours);
    mnuLandscape->addSeparator();
    mnuLandscape->addAction(actHmSettings);
    mnuLandscape->addAction(actExtrapolSettings);
    mnuLandscape->addAction(actContourSettings);

    QActionGroup *agpViewMode = new QActionGroup(master);
    QObject::connect(agpViewMode, SIGNAL(triggered(QAction *)), master, SLOT(setViewMode(QAction *)));

    QAction *actViewModePeaks = new QAction(agpViewMode);
    actViewModePeaks->setText(HeightMapWindow::tr("Peaks"));
    actViewModePeaks->setCheckable(true);
    actViewModePeaks->setProperty("hmvm", HMVM_Peaks);

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
    actViewModeHybrid->setChecked(true);
    actViewModeHybrid->setProperty("hmvm", HMVM_Hybrid);

    QMenu *mnuView = master->menuBar()->addMenu(HeightMapWindow::tr("&View"));
    mnuView->addAction(actViewModePeaks);
    mnuView->addAction(actViewModeLandscape);
    mnuView->addAction(actViewModeIsobars);
    mnuView->addAction(actViewModeHybrid);

    QMenu *mnuWindows = master->menuBar()->addMenu(HeightMapWindow::tr("&Windows"));
    mnuWindows->addAction(dckPeakGenerating->toggleViewAction());
    mnuWindows->addAction(dckExtrapolation->toggleViewAction());
    mnuWindows->addAction(dckContouring->toggleViewAction());
}

void HeightMapWindowImplementation::createWidgets()
{
    Preferences prefs = hmApp->preferences();
    PreferencesController *ctrl = hmApp->preferencesController();

    wgtPeakGenerating = new PeakOptionsWidget;
    wgtPeakGenerating->setRange(prefs.minPeak(), prefs.maxPeak());
    wgtPeakGenerating->setPeakCount(prefs.peakCount());

    wgtExtrapolation = new ExtrapolationOptionsWidget;

    QStringList xNames = hmApp->extrapolatorKeys();
    for (QStringList::ConstIterator i = xNames.begin(); i != xNames.end(); ++i) {
        if (AbstractExtrapolationWidget *xWidget = hmApp->createExtrapolationWidget(*i)) {
            xWidget->setDirectInfluence(true);
            wgtExtrapolation->addExtrapolationWidget(*i, hmApp->extrapolationDescription(*i), xWidget);
        }
    }

    wgtExtrapolation->setExtrapolatorName(prefs.extrapolatorName());

    wgtContouring = new ContouringOptionsWidget;
    wgtContouring->setLevelRange(prefs.minContouringLevel(), prefs.maxContouringLevel());
    wgtContouring->setStep(prefs.contouringStep());

    HeightMapWindow::connect(wgtPeakGenerating, SIGNAL(minPeakChanged(int)), ctrl, SLOT(setMinPeak(int)));
    HeightMapWindow::connect(wgtPeakGenerating, SIGNAL(maxPeakChanged(int)), ctrl, SLOT(setMaxPeak(int)));
    HeightMapWindow::connect(wgtPeakGenerating, SIGNAL(peakCountChanged(int)), ctrl, SLOT(setPeakCount(int)));

    HeightMapWindow::connect(
                wgtExtrapolation, SIGNAL(extrapolatorNameChanged(QString)), ctrl, SLOT(setExtrapolatorName(QString)));

    HeightMapWindow::connect(wgtContouring, SIGNAL(minLevelChanged(int)), ctrl, SLOT(setMinContouringLevel(int)));
    HeightMapWindow::connect(wgtContouring, SIGNAL(maxLevelChanged(int)), ctrl, SLOT(setMaxContouringLevel(int)));
    HeightMapWindow::connect(wgtContouring, SIGNAL(stepChanged(int)), ctrl, SLOT(setContouringStep(int)));
}

void HeightMapWindowImplementation::createDocks(HeightMapWindow *master)
{
    dckPeakGenerating = new QDockWidget(master);
    dckPeakGenerating->setWindowTitle(HeightMapWindow::tr("Peak settings"));
    dckPeakGenerating->setWidget(wgtPeakGenerating);
    master->addDockWidget(Qt::LeftDockWidgetArea, dckPeakGenerating);

    dckExtrapolation = new QDockWidget(master);
    dckExtrapolation->setWindowTitle(HeightMapWindow::tr("Extrapolation settings"));
    dckExtrapolation->setWidget(wgtExtrapolation);
    master->addDockWidget(Qt::LeftDockWidgetArea, dckExtrapolation);

    dckContouring = new QDockWidget(master);
    dckContouring->setWindowTitle(HeightMapWindow::tr("Contouring settings"));
    dckContouring->setWidget(wgtContouring);
    master->addDockWidget(Qt::LeftDockWidgetArea, dckContouring);
}

void HeightMapWindowImplementation::provideMappingData(MappingWorker *worker)
{
    MappingData hmData = {
        &terrain,
        &imgPeaks,
        &imgLandscape,
        &imgIsobars,
        &imgHybrid
    };

    worker->initFrom(hmData);
}

void HeightMapWindowImplementation::provideExtrapolationWidgets(ExtrapolationOptionsDialog *dialog)
{
    QStringList xNames = hmApp->extrapolatorKeys();
    for (QStringList::ConstIterator i = xNames.begin(); i != xNames.end(); ++i) {
        if (AbstractExtrapolationWidget *xWidget = hmApp->createExtrapolationWidget(*i))
            dialog->addExtrapolationWidget(*i, hmApp->extrapolationDescription(*i), xWidget);
    }
}

void HeightMapWindowImplementation::displayHeightMapImage()
{
    switch (hmvm) {
    case HMVM_Peaks:
        hmImgLabel->setPixmap(QPixmap::fromImage(imgPeaks));
        break;
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

void HeightMapWindowImplementation::resetImages()
{
    imgPeaks = QImage(terrain.width(), terrain.height(), QImage::Format_ARGB32_Premultiplied);
    imgLandscape = QImage(terrain.width(), terrain.height(), QImage::Format_ARGB32_Premultiplied);
    imgIsobars = QImage(terrain.width(), terrain.height(), QImage::Format_ARGB32_Premultiplied);
    imgHybrid = QImage(terrain.width(), terrain.height(), QImage::Format_ARGB32_Premultiplied);

    imgPeaks.fill(Qt::transparent);
    imgLandscape.fill(Qt::transparent);
    imgIsobars.fill(Qt::transparent);
    imgHybrid.fill(Qt::transparent);
}

void HeightMapWindowImplementation::resetStatusBar()
{
    lsSizeLabel->setText(QString("%1%2%3")
                         .arg(terrain.width())
                         .arg(QChar(0x00d7))
                         .arg(terrain.height()));

    lvlsLabel->clear();
    pkLabel->clear();
    cntrsLabel->clear();
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
    MappingWorker *worker = new MappingWorker;
    m->provideMappingData(worker);
    worker->moveToThread(&m->procThread);

    m->createWidgets();
    m->createDocks(this);
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

    m->resetStatusBar();

    connect(hmApp, SIGNAL(preferencesChanged()), this, SLOT(adjustPreferences()));

    connect(worker, SIGNAL(processStarted()), this, SLOT(onProcessStarted()), Qt::BlockingQueuedConnection);
    connect(worker, SIGNAL(processFinished()), this, SLOT(onProcessFinished()), Qt::BlockingQueuedConnection);

    connect(worker, SIGNAL(peakGeneratingStarted()), this, SLOT(onPeakGeneratingStarted()), Qt::BlockingQueuedConnection);
    connect(worker, SIGNAL(peakGeneratingFinished()), this, SLOT(onPeakGeneratingFinished()), Qt::BlockingQueuedConnection);
    connect(worker, SIGNAL(peakExtrapolationStarted()), this, SLOT(onPeakExtrapolationStarted()), Qt::BlockingQueuedConnection);
    connect(worker, SIGNAL(peakExtrapolated(QPoint, double)), this, SLOT(onPeakExtrapolated(QPoint, double)));
    connect(worker, SIGNAL(peakExtrapolationFinished()), this, SLOT(onPeakExtrapolationFinished()), Qt::BlockingQueuedConnection);
    connect(worker, SIGNAL(contouringStarted()), this, SLOT(onContouringStarted()), Qt::BlockingQueuedConnection);
    connect(worker, SIGNAL(contouringLevelsAcquired(int)), this, SLOT(onContouringLevelsAcquired(int)), Qt::BlockingQueuedConnection);
    connect(worker, SIGNAL(contouringAt(int)), this, SLOT(onContouringAt(int)));
    connect(worker, SIGNAL(contouringFinished()), this, SLOT(onContouringFinished()), Qt::BlockingQueuedConnection);

    connect(&m->procThread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    m->procThread.start();
}

HeightMapWindow::~HeightMapWindow()
{
    delete m;
}


void HeightMapWindow::newFile()
{
    TerrainOptionsDialog dialog(this);
    dialog.setWindowTitle(tr("New file"));
    dialog.setPreferences(hmApp->preferences());

    if (!dialog.exec())
        return;

    hmApp->setPreferences(dialog.preferences());

    Preferences prefs = hmApp->preferences();
    m->terrain = Terrain(prefs.landscapeWidth(), prefs.landscapeHeight());

    m->resetImages();
    m->resetStatusBar();
    m->displayHeightMapImage();
}

void HeightMapWindow::exportLandscape()
{
    QFileDialog dialog(this);
    dialog.setWindowTitle(tr("Export landscape"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setOptions(QFileDialog::DontResolveSymlinks);
    dialog.setDirectory(QDir::home());
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("HeightMap landscape text files (*.%1)").arg(LS_TEXT_FILE_EXT));
    dialog.setViewMode(QFileDialog::Detail);

    if (!dialog.exec())
        return;

    QString filename(dialog.selectedFiles().value(0));

    std::ofstream stream;
    stream.open(filename.toStdString(), std::ios::out | std::ios::trunc);
    m->terrain.exportLandscape(stream, 4);
    stream.close();
}

void HeightMapWindow::exportPeaks()
{
    QFileDialog dialog(this);
    dialog.setWindowTitle(tr("Export peaks"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setOptions(QFileDialog::DontResolveSymlinks);
    dialog.setDirectory(QDir::home());
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("HeightMap peak text files (*.%1)").arg(PK_TEXT_FILE_EXT));
    dialog.setViewMode(QFileDialog::Detail);

    if (!dialog.exec())
        return;

    QString filename(dialog.selectedFiles().value(0));

    std::ofstream stream;
    stream.open(filename.toStdString(), std::ios::out | std::ios::trunc);
    m->terrain.exportPeaks(stream);
    stream.close();
}

void HeightMapWindow::editPeakSettings()
{
    Preferences prefs(hmApp->preferences());

    PreferencesController ctrl;
    ctrl.setPreferences(&prefs);

    GeneratingOptionsDialog dialog(this);
    dialog.setWindowTitle(tr("Peak settings"));
    dialog.setPreferencesController(&ctrl);

    if (dialog.exec()) {
        hmApp->setPreferences(prefs);
    }
}

void HeightMapWindow::editExtrapolationSettings()
{
    Preferences prefs(hmApp->preferences());

    PreferencesController ctrl;
    ctrl.setPreferences(&prefs);

    ExtrapolationOptionsDialog dialog(this);
    m->provideExtrapolationWidgets(&dialog);
    dialog.setWindowTitle(tr("Extrapolation settings"));
    dialog.setPreferencesController(&ctrl);

    if (dialog.exec()) {
        hmApp->setPreferences(prefs);
    }
}

void HeightMapWindow::editContouringSettings()
{
    Preferences prefs(hmApp->preferences());

    PreferencesController ctrl;
    ctrl.setPreferences(&prefs);

    ContouringOptionsDialog dialog(this);
    dialog.setWindowTitle(tr("Contouring settings"));
    dialog.setPreferencesController(&ctrl);

    if (dialog.exec()) {
        hmApp->setPreferences(prefs);
    }
}

void HeightMapWindow::adjustPreferences()
{
    Preferences prefs(hmApp->preferences());

    m->wgtPeakGenerating->setRange(prefs.minPeak(), prefs.maxPeak());
    m->wgtPeakGenerating->setPeakCount(prefs.peakCount());
    m->wgtExtrapolation->retrieveExtrapolationSettings();
    m->wgtExtrapolation->setExtrapolatorName(prefs.extrapolatorName());
    m->wgtContouring->setLevelRange(prefs.minContouringLevel(), prefs.maxContouringLevel());
    m->wgtContouring->setStep(prefs.contouringStep());
}

void HeightMapWindow::onProcessStarted()
{
    m->stateLabel->setText(tr("Processing..."));
    m->procBar->setValue(0);
    m->procBar->setMaximum(static_cast<int>(hmApp->preferences().peakCount()) + m->terrain.width() - 1);
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
    m->pkLabel->setText(tr("%1 peak(s)").arg(m->terrain.peaks().size()));
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

void HeightMapWindow::onContouringLevelsAcquired(int levels)
{
    m->lvlsLabel->setText(tr("%1 level(s)").arg(levels));
}

void HeightMapWindow::onContouringAt(int)
{
    m->procBar->setValue(m->procBar->value() + 1);
}

void HeightMapWindow::onContouringFinished()
{
    m->cntrsLabel->setText(tr("%1 isobar segment(s)").arg(m->terrain.contours().size()));
}

void HeightMapWindow::setViewMode(QAction *viewModeAct)
{
    m->hmvm = static_cast<HeightMapViewMode>(viewModeAct->property("hmvm").toInt());
    m->displayHeightMapImage();
}


} // namespace HeightMap
