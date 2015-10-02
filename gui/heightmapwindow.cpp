#include <QLabel>
#include <QProgressBar>
#include <QScrollArea>
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QBoxLayout>
#include <QPixmap>
#include <QFileDialog>
#include <fstream>
#include "heightmaplogic.h"
#include "heightmapapplication.h"
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
const char *LS_TEXT_FILE_EXT = "hmlst";
const char *PK_TEXT_FILE_EXT = "hmpkt";
}


struct HeightMapWindowImplementation
{
    HeightMapWindowImplementation();

    void createActions(HeightMapWindow *master);
    void createWidgets();
    void createDocks(HeightMapWindow *master);

    void provideExtrapolationWidgets(ExtrapolationOptionsDialog *);

    void displayHeightMapImage();
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

    HeightMapViewMode hmvm;

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
      hmvm(HMVM_Hybrid),
      processing(false) { }

void HeightMapWindowImplementation::createActions(HeightMapWindow *master)
{
    QAction *actNewFile = new QAction(master);
    actNewFile->setText(HeightMapWindow::tr("&New file..."));
    actNewFile->setShortcut(HeightMapWindow::tr("Ctrl+N"));
    QObject::connect(actNewFile, &QAction::triggered, master, &HeightMapWindow::newFile);

    QAction *actExportLs = new QAction(master);
    actExportLs->setText(HeightMapWindow::tr("Export landscape..."));
    QObject::connect(actExportLs, &QAction::triggered, master, &HeightMapWindow::exportLandscape);

    QAction *actExportPk = new QAction(master);
    actExportPk->setText(HeightMapWindow::tr("Export peaks..."));
    QObject::connect(actExportPk, &QAction::triggered, master, &HeightMapWindow::exportPeaks);

    QAction *actExit = new QAction(master);
    actExit->setText(HeightMapWindow::tr("E&xit"));
    actExit->setShortcut(HeightMapWindow::tr("Alt+X"));
    QObject::connect(actExit, &QAction::triggered, master, &HeightMapWindow::close);

    QMenu *mnuFile = master->menuBar()->addMenu(HeightMapWindow::tr("&File"));
    mnuFile->addAction(actNewFile);
    mnuFile->addSeparator();
    mnuFile->addAction(actExportLs);
    mnuFile->addAction(actExportPk);
    mnuFile->addSeparator();
    mnuFile->addAction(actExit);

    HeightMapLogic *logic = hmApp->logic();

    QAction *actGenLs = new QAction(master);
    actGenLs->setText(HeightMapWindow::tr("&Generate landscape"));
    actGenLs->setShortcut(HeightMapWindow::tr("Ctrl+G"));
    QObject::connect(actGenLs, &QAction::triggered, logic, &HeightMapLogic::createLandscape);

    QAction *actBuildLs = new QAction(master);
    actBuildLs->setText(HeightMapWindow::tr("E&xtrapolate peaks"));
    actBuildLs->setShortcut(HeightMapWindow::tr("Ctrl+E"));
    QObject::connect(actBuildLs, &QAction::triggered, logic, &HeightMapLogic::buildLandscapeFromPeaks);

    QAction *actCalcContours = new QAction(master);
    actCalcContours->setText(HeightMapWindow::tr("Calculate &isobars"));
    actCalcContours->setShortcut(HeightMapWindow::tr("Ctrl+I"));
    QObject::connect(actCalcContours, &QAction::triggered,  logic, &HeightMapLogic::plotIsobars);

    QAction *actHmSettings = new QAction(master);
    actHmSettings->setText(HeightMapWindow::tr("&Peak settings..."));
    actHmSettings->setShortcut(HeightMapWindow::tr("F6"));
    QObject::connect(actHmSettings, &QAction::triggered, master, &HeightMapWindow::editPeakSettings);

    QAction *actExtrapolSettings = new QAction(master);
    actExtrapolSettings->setText(HeightMapWindow::tr("&Extrapolation settings..."));
    actExtrapolSettings->setShortcut(HeightMapWindow::tr("F7"));
    QObject::connect(actExtrapolSettings, &QAction::triggered, master, &HeightMapWindow::editExtrapolationSettings);

    QAction *actContourSettings = new QAction(master);
    actContourSettings->setText(HeightMapWindow::tr("&Contouring settings..."));
    actContourSettings->setShortcut(HeightMapWindow::tr("F8"));
    QObject::connect(actContourSettings, &QAction::triggered, master, &HeightMapWindow::editContouringSettings);

    QMenu *mnuLandscape = master->menuBar()->addMenu(HeightMapWindow::tr("&Landscape"));
    mnuLandscape->addAction(actGenLs);
    mnuLandscape->addAction(actBuildLs);
    mnuLandscape->addAction(actCalcContours);
    mnuLandscape->addSeparator();
    mnuLandscape->addAction(actHmSettings);
    mnuLandscape->addAction(actExtrapolSettings);
    mnuLandscape->addAction(actContourSettings);

    QActionGroup *agpViewMode = new QActionGroup(master);
    QObject::connect(agpViewMode, &QActionGroup::triggered, master, &HeightMapWindow::setViewMode);

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
    hmImgLabel->setPixmap(QPixmap::fromImage(hmApp->logic()->heightMapImage(hmvm)));
}

void HeightMapWindowImplementation::resetStatusBar()
{
    Terrain *terrain = hmApp->logic()->terrain();
    lsSizeLabel->setText(QString("%1%2%3")
                         .arg(terrain->width())
                         .arg(QChar(0x00d7))
                         .arg(terrain->height()));

    lvlsLabel->setText(QString());
    pkLabel->setText(QString());
    cntrsLabel->setText(QString());
}

HeightMapWindowImplementation::~HeightMapWindowImplementation() { }


HeightMapWindow::HeightMapWindow(QWidget *parent)
    : QMainWindow(parent),
      m(new HeightMapWindowImplementation)
{
    m->createWidgets();
    m->createDocks(this);
    m->createActions(this);

    m->hmImgLabel->setAlignment(Qt::AlignCenter);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
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

    connect(hmApp, &HeightMapApplication::preferencesChanged,   this, &HeightMapWindow::adjustPreferences);

    HeightMapLogic *logic = hmApp->logic();

    connect(logic, &HeightMapLogic::terrainCreated,             this, &HeightMapWindow::resetTerrainData);
    connect(logic, &HeightMapLogic::processStarted,             this, &HeightMapWindow::onProcessStarted);
    connect(logic, &HeightMapLogic::processFinished,            this, &HeightMapWindow::onProcessFinished);
    connect(logic, &HeightMapLogic::peakGeneratingStarted,      this, &HeightMapWindow::onPeakGeneratingStarted);
    connect(logic, &HeightMapLogic::peakGeneratingFinished,     this, &HeightMapWindow::onPeakGeneratingFinished);
    connect(logic, &HeightMapLogic::peakExtrapolationStarted,   this, &HeightMapWindow::onPeakExtrapolationStarted);
    connect(logic, &HeightMapLogic::peakExtrapolated,           this, &HeightMapWindow::onPeakExtrapolated);
    connect(logic, &HeightMapLogic::peakExtrapolationFinished,  this, &HeightMapWindow::onPeakExtrapolationFinished);
    connect(logic, &HeightMapLogic::contouringStarted,          this, &HeightMapWindow::onContouringStarted);
    connect(logic, &HeightMapLogic::contouringLevelsAcquired,   this, &HeightMapWindow::onContouringLevelsAcquired);
    connect(logic, &HeightMapLogic::contouringAt,               this, &HeightMapWindow::onContouringAt);
    connect(logic, &HeightMapLogic::contouringFinished,         this, &HeightMapWindow::onContouringFinished);
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
    hmApp->logic()->newTerrain();
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
    hmApp->logic()->terrain()->exportLandscape(stream, 4);
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
    hmApp->logic()->terrain()->exportPeaks(stream);
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
        m->wgtExtrapolation->retrieveExtrapolationSettings();
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
    m->wgtExtrapolation->setExtrapolatorName(prefs.extrapolatorName());
    m->wgtContouring->setLevelRange(prefs.minContouringLevel(), prefs.maxContouringLevel());
    m->wgtContouring->setStep(prefs.contouringStep());
}

void HeightMapWindow::resetTerrainData()
{
    m->resetStatusBar();
    m->displayHeightMapImage();
}

void HeightMapWindow::onProcessStarted()
{
    Terrain *terrain = hmApp->logic()->terrain();

    m->stateLabel->setText(tr("Processing..."));
    m->procBar->setValue(0);
    m->procBar->setMaximum(static_cast<int>(hmApp->preferences().peakCount()) + terrain->width() - 1);
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
    Terrain *terrain = hmApp->logic()->terrain();
    m->pkLabel->setText(tr("%1 peak(s)").arg(terrain->peaks().size()));
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
    Terrain *terrain = hmApp->logic()->terrain();
    m->cntrsLabel->setText(tr("%1 isobar segment(s)").arg(terrain->contours().size()));
}

void HeightMapWindow::setViewMode(QAction *viewModeAct)
{
    m->hmvm = static_cast<HeightMapViewMode>(viewModeAct->property("hmvm").toInt());
    m->displayHeightMapImage();
}


} // namespace HeightMap
