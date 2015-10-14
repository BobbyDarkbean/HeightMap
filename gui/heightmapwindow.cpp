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
#include "../io/landscapetextiohandler.h"
#include "../io/peakstextiohandler.h"
#include "heightmapapplication.h"
#include "heightmaplogic.h"
#include "preferences.h"
#include "preferencescontroller.h"
#include "terrain.h"
#include "extrapolation/extrapolationdata.h"
#include "extrapolation/extrapolationfactory.h"
#include "commands/commands"
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

    void provideExtrapolationWidgets(ExtrapolationOptionsDialog *);

    void displayHeightMapImage();
    void resetStatusBar();

    ~HeightMapWindowImplementation();

    HeightMapLogic *logic;

    QLabel *lblHmImg;

    PeakOptionsWidget *wgtPeakGenerating;
    ExtrapolationOptionsWidget *wgtExtrapolation;
    ContouringOptionsWidget *wgtContouring;

    QDockWidget *dckPeakGenerating;
    QDockWidget *dckExtrapolation;
    QDockWidget *dckContouring;

    QLabel *lblState;
    QLabel *lblLandscape;
    QLabel *lblLevels;
    QLabel *lblProcess;
    QProgressBar *prgProcess;
    QLabel *lblPeaks;
    QLabel *lblIsobars;

    QAction *actNewFile;
    QAction *actExportLs;
    QAction *actExportPk;
    QAction *actExit;

    QAction *actUndo;
    QAction *actRedo;

    QAction *actGenLs;
    QAction *actBuildLs;
    QAction *actCalcContours;
    QAction *actHmSettings;
    QAction *actExtrapolSettings;
    QAction *actContourSettings;

    QAction *actViewModePeaks;
    QAction *actViewModeLandscape;
    QAction *actViewModeIsobars;
    QAction *actViewModeHybrid;

    QUndoStack *uskCommands;

    HeightMapViewMode hmvm;

    bool processing;

private:
    DISABLE_COPY(HeightMapWindowImplementation)
    DISABLE_MOVE(HeightMapWindowImplementation)
};


HeightMapWindowImplementation::HeightMapWindowImplementation()
    : logic(),
      lblHmImg(new QLabel),
      wgtPeakGenerating(nullptr),
      wgtExtrapolation(nullptr),
      wgtContouring(nullptr),
      dckPeakGenerating(nullptr),
      dckExtrapolation(nullptr),
      dckContouring(nullptr),
      lblState(new QLabel),
      lblLandscape(new QLabel),
      lblLevels(new QLabel),
      lblProcess(new QLabel),
      prgProcess(new QProgressBar),
      lblPeaks(new QLabel),
      lblIsobars(new QLabel),
      actNewFile(nullptr),
      actExportLs(nullptr),
      actExportPk(nullptr),
      actExit(nullptr),
      actUndo(nullptr),
      actRedo(nullptr),
      actGenLs(nullptr),
      actBuildLs(nullptr),
      actCalcContours(nullptr),
      actHmSettings(nullptr),
      actExtrapolSettings(nullptr),
      actContourSettings(nullptr),
      actViewModePeaks(nullptr),
      actViewModeLandscape(nullptr),
      actViewModeIsobars(nullptr),
      actViewModeHybrid(nullptr),
      uskCommands(nullptr),
      hmvm(HMVM_Hybrid),
      processing(false) { }

void HeightMapWindowImplementation::provideExtrapolationWidgets(ExtrapolationOptionsDialog *dialog)
{
    QStringList xNames = hmApp->extrapolatorKeys();
    foreach (QString name, xNames) {
        if (ExtrapolationFactory *f = hmApp->extrapolationFactory(name))
            dialog->addExtrapolationWidget(f);
    }
}

void HeightMapWindowImplementation::displayHeightMapImage()
{
    lblHmImg->setPixmap(QPixmap::fromImage(logic->heightMapImage(hmvm)));
}

void HeightMapWindowImplementation::resetStatusBar()
{
    Terrain *terrain = logic->terrain();
    lblLandscape->setText(QString("%1%2%3")
                          .arg(terrain->width())
                          .arg(QChar(0x00d7))
                          .arg(terrain->height()));

    lblLevels->setText(QString());
    lblPeaks->setText(QString());
    lblIsobars->setText(QString());
}

HeightMapWindowImplementation::~HeightMapWindowImplementation() { }


HeightMapWindow::HeightMapWindow(QWidget *parent)
    : QMainWindow(parent),
      m(new HeightMapWindowImplementation)
{
}


void HeightMapWindow::init(HeightMapLogic *l)
{
    m->logic = l;

    createCentral();
    createWidgets();
    createDocks();
    createActions();
    createStatusBar();

    typedef HeightMapApplication A;
    typedef HeightMapWindow W;
    typedef HeightMapLogic L;

    connect(hmApp,      &A::preferencesChanged,         this,   &W::adjustPreferences);
    connect(hmApp,      &A::extrapolationDataChanged,   this,   &W::adjustExtrapolationData);

    connect(m->logic,   &L::terrainCreated,             this,   &W::resetTerrainData);
    connect(m->logic,   &L::processStarted,             this,   &W::onProcessStarted);
    connect(m->logic,   &L::processFinished,            this,   &W::onProcessFinished);
    connect(m->logic,   &L::peakGeneratingStarted,      this,   &W::onPeakGeneratingStarted);
    connect(m->logic,   &L::peakGeneratingFinished,     this,   &W::onPeakGeneratingFinished);
    connect(m->logic,   &L::peakExtrapolationStarted,   this,   &W::onPeakExtrapolationStarted);
    connect(m->logic,   &L::peakExtrapolated,           this,   &W::onPeakExtrapolated);
    connect(m->logic,   &L::peakExtrapolationFinished,  this,   &W::onPeakExtrapolationFinished);
    connect(m->logic,   &L::contouringStarted,          this,   &W::onContouringStarted);
    connect(m->logic,   &L::contouringLevelsAcquired,   this,   &W::onContouringLevelsAcquired);
    connect(m->logic,   &L::contouringAt,               this,   &W::onContouringAt);
    connect(m->logic,   &L::contouringFinished,         this,   &W::onContouringFinished);
}


HeightMapWindow::~HeightMapWindow()
{
    delete m;
}


void HeightMapWindow::newFile()
{
    TerrainOptionsDialog dialog(this);
    dialog.setWindowTitle(tr("New file"));
    dialog.setPreferences(m->logic->preferences());

    if (!dialog.exec())
        return;

    hmApp->setPreferences(dialog.preferences());
    m->logic->newTerrain();
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

    LandscapeTextIOHandler ioHandler;
    ioHandler.write(m->logic->terrain(), filename);
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

    PeaksTextIOHandler ioHandler;
    ioHandler.write(m->logic->terrain(), filename);
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
        hmApp->setXData(prefs.extrapolatorName(), dialog.xData());
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

void HeightMapWindow::generateLandscape()
{
    GenerateCommand *cmd = new GenerateCommand;
    cmd->init(m->logic);
    cmd->setText(tr("Generate peaks"));
    cmd->setPreferences(hmApp->preferences());
    if (ExtrapolationFactory *f = hmApp->currentExtrapolationFactory()) {
        cmd->setXData(f->extractData());
    }

    m->uskCommands->push(cmd);
    m->uskCommands->redo();
}

void HeightMapWindow::extrapolatePeaks()
{
    ExtrapolateCommand *cmd = new ExtrapolateCommand;
    cmd->init(m->logic);
    cmd->setText(tr("Extrapolate peaks"));
    cmd->setPreferences(hmApp->preferences());
    if (ExtrapolationFactory *f = hmApp->currentExtrapolationFactory()) {
        cmd->setXData(f->extractData());
    }

    m->uskCommands->push(cmd);
    m->uskCommands->redo();
}

void HeightMapWindow::calculateContours()
{
    ContouringCommand *cmd = new ContouringCommand;
    cmd->init(m->logic);
    cmd->setText(tr("Calculate contours"));
    cmd->setPreferences(hmApp->preferences());

    m->uskCommands->push(cmd);
    m->uskCommands->redo();
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

void HeightMapWindow::adjustExtrapolationData(QString)
{
    m->wgtExtrapolation->retrieveExtrapolationSettings();
}

void HeightMapWindow::resetTerrainData()
{
    m->uskCommands->clear();

    m->resetStatusBar();
    m->displayHeightMapImage();
}

void HeightMapWindow::onProcessStarted()
{
    m->lblState->setText(tr("Processing..."));
    m->prgProcess->setValue(0);
    m->prgProcess->setMaximum(static_cast<int>(m->logic->preferences().peakCount()) +
                              m->logic->terrain()->width() - 1);
    m->prgProcess->show();

    m->processing = true;
}

void HeightMapWindow::onProcessFinished()
{
    m->processing = false;

    m->lblState->setText(tr("Done"));
    m->lblProcess->clear();
    m->prgProcess->hide();

    m->displayHeightMapImage();
}

void HeightMapWindow::onPeakGeneratingStarted()
{
    m->lblProcess->setText("Generating peaks...");
}

void HeightMapWindow::onPeakGeneratingFinished()
{
    Terrain *terrain = m->logic->terrain();
    m->lblPeaks->setText(tr("%1 peak(s)").arg(terrain->peaks().size()));
}

void HeightMapWindow::onPeakExtrapolationStarted()
{
    m->lblProcess->setText("Extrapolating peaks...");
}

void HeightMapWindow::onPeakExtrapolated(QPoint, double)
{
    m->prgProcess->setValue(m->prgProcess->value() + 1);
}

void HeightMapWindow::onPeakExtrapolationFinished()
{
}

void HeightMapWindow::onContouringStarted()
{
    m->lblProcess->setText("Calculating contours...");
}

void HeightMapWindow::onContouringLevelsAcquired(int levels)
{
    m->lblLevels->setText(tr("%1 level(s)").arg(levels));
}

void HeightMapWindow::onContouringAt(int)
{
    m->prgProcess->setValue(m->prgProcess->value() + 1);
}

void HeightMapWindow::onContouringFinished()
{
    Terrain *terrain = m->logic->terrain();
    m->lblIsobars->setText(tr("%1 isobar segment(s)").arg(terrain->contours().size()));
}

void HeightMapWindow::setViewMode(QAction *viewModeAct)
{
    m->hmvm = static_cast<HeightMapViewMode>(viewModeAct->property("hmvm").toInt());
    m->displayHeightMapImage();
}


void HeightMapWindow::createCentral()
{
    m->lblHmImg->setAlignment(Qt::AlignCenter);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(m->lblHmImg);
    setCentralWidget(scrollArea);
}

void HeightMapWindow::createWidgets()
{
    PreferencesController *ctrl = hmApp->preferencesController();
    Preferences prefs = *ctrl->preferences();

    m->wgtPeakGenerating = new PeakOptionsWidget;
    m->wgtPeakGenerating->setRange(prefs.minPeak(), prefs.maxPeak());
    m->wgtPeakGenerating->setPeakCount(prefs.peakCount());

    m->wgtExtrapolation = new ExtrapolationOptionsWidget;

    QStringList xNames = hmApp->extrapolatorKeys();
    foreach (QString name, xNames) {
        if (ExtrapolationFactory *f = hmApp->extrapolationFactory(name))
            m->wgtExtrapolation->addExtrapolationWidget(f, true);
    }

    m->wgtExtrapolation->setExtrapolatorName(prefs.extrapolatorName());

    m->wgtContouring = new ContouringOptionsWidget;
    m->wgtContouring->setLevelRange(prefs.minContouringLevel(), prefs.maxContouringLevel());
    m->wgtContouring->setStep(prefs.contouringStep());

    typedef PeakOptionsWidget P;
    typedef ExtrapolationOptionsWidget X;
    typedef ContouringOptionsWidget C;
    typedef PreferencesController PCtrl;

    connect(m->wgtPeakGenerating,   &P::minPeakChanged,             ctrl,   &PCtrl::setMinPeak);
    connect(m->wgtPeakGenerating,   &P::maxPeakChanged,             ctrl,   &PCtrl::setMaxPeak);
    connect(m->wgtPeakGenerating,   &P::peakCountChanged,           ctrl,   &PCtrl::setPeakCount);

    connect(m->wgtExtrapolation,    &X::extrapolatorNameChanged,    ctrl,   &PCtrl::setExtrapolatorName);

    connect(m->wgtContouring,       &C::minLevelChanged,            ctrl,   &PCtrl::setMinContouringLevel);
    connect(m->wgtContouring,       &C::maxLevelChanged,            ctrl,   &PCtrl::setMaxContouringLevel);
    connect(m->wgtContouring,       &C::stepChanged,                ctrl,   &PCtrl::setContouringStep);
}

void HeightMapWindow::createDocks()
{
    m->dckPeakGenerating = new QDockWidget(this);
    m->dckPeakGenerating->setWindowTitle(tr("Peak settings"));
    m->dckPeakGenerating->setWidget(m->wgtPeakGenerating);
    addDockWidget(Qt::LeftDockWidgetArea, m->dckPeakGenerating);

    m->dckExtrapolation = new QDockWidget(this);
    m->dckExtrapolation->setWindowTitle(tr("Extrapolation settings"));
    m->dckExtrapolation->setWidget(m->wgtExtrapolation);
    addDockWidget(Qt::LeftDockWidgetArea, m->dckExtrapolation);

    m->dckContouring = new QDockWidget(this);
    m->dckContouring->setWindowTitle(tr("Contouring settings"));
    m->dckContouring->setWidget(m->wgtContouring);
    addDockWidget(Qt::LeftDockWidgetArea, m->dckContouring);
}

void HeightMapWindow::createActions()
{
    m->actNewFile = new QAction(this);
    m->actNewFile->setText(tr("&New file..."));
    m->actNewFile->setShortcut(tr("Ctrl+N"));

    m->actExportLs = new QAction(this);
    m->actExportLs->setText(tr("Export landscape..."));

    m->actExportPk = new QAction(this);
    m->actExportPk->setText(tr("Export peaks..."));

    m->actExit = new QAction(this);
    m->actExit->setText(tr("E&xit"));
    m->actExit->setShortcut(tr("Alt+X"));

    QMenu *mnuFile = menuBar()->addMenu(tr("&File"));
    mnuFile->addAction(m->actNewFile);
    mnuFile->addSeparator();
    mnuFile->addAction(m->actExportLs);
    mnuFile->addAction(m->actExportPk);
    mnuFile->addSeparator();
    mnuFile->addAction(m->actExit);

    m->uskCommands = new QUndoStack(this);

    m->actUndo = m->uskCommands->createUndoAction(this);
    m->actUndo->setShortcut(tr("Ctrl+Z"));

    m->actRedo = m->uskCommands->createRedoAction(this);
    m->actRedo->setShortcut(tr("Ctrl+Shift+Z"));

    QMenu *mnuEdit = menuBar()->addMenu(tr("&Edit"));
    mnuEdit->addAction(m->actUndo);
    mnuEdit->addAction(m->actRedo);

    m->actGenLs = new QAction(this);
    m->actGenLs->setText(tr("&Generate landscape"));
    m->actGenLs->setShortcut(tr("Ctrl+G"));

    m->actBuildLs = new QAction(this);
    m->actBuildLs->setText(tr("E&xtrapolate peaks"));
    m->actBuildLs->setShortcut(tr("Ctrl+E"));

    m->actCalcContours = new QAction(this);
    m->actCalcContours->setText(tr("Calculate &isobars"));
    m->actCalcContours->setShortcut(tr("Ctrl+I"));

    m->actHmSettings = new QAction(this);
    m->actHmSettings->setText(tr("&Peak settings..."));
    m->actHmSettings->setShortcut(tr("F6"));

    m->actExtrapolSettings = new QAction(this);
    m->actExtrapolSettings->setText(tr("&Extrapolation settings..."));
    m->actExtrapolSettings->setShortcut(tr("F7"));

    m->actContourSettings = new QAction(this);
    m->actContourSettings->setText(tr("&Contouring settings..."));
    m->actContourSettings->setShortcut(tr("F8"));

    QMenu *mnuLandscape = menuBar()->addMenu(tr("&Landscape"));
    mnuLandscape->addAction(m->actGenLs);
    mnuLandscape->addAction(m->actBuildLs);
    mnuLandscape->addAction(m->actCalcContours);
    mnuLandscape->addSeparator();
    mnuLandscape->addAction(m->actHmSettings);
    mnuLandscape->addAction(m->actExtrapolSettings);
    mnuLandscape->addAction(m->actContourSettings);

    QActionGroup *agpViewMode = new QActionGroup(this);

    m->actViewModePeaks = new QAction(agpViewMode);
    m->actViewModePeaks->setText(tr("Peaks"));
    m->actViewModePeaks->setCheckable(true);
    m->actViewModePeaks->setProperty("hmvm", HMVM_Peaks);

    m->actViewModeLandscape = new QAction(agpViewMode);
    m->actViewModeLandscape->setText(tr("Landscape"));
    m->actViewModeLandscape->setCheckable(true);
    m->actViewModeLandscape->setProperty("hmvm", HMVM_Landscape);

    m->actViewModeIsobars = new QAction(agpViewMode);
    m->actViewModeIsobars->setText(tr("Isobars"));
    m->actViewModeIsobars->setCheckable(true);
    m->actViewModeIsobars->setProperty("hmvm", HMVM_Isobars);

    m->actViewModeHybrid = new QAction(agpViewMode);
    m->actViewModeHybrid->setText(tr("Hybrid"));
    m->actViewModeHybrid->setCheckable(true);
    m->actViewModeHybrid->setChecked(true);
    m->actViewModeHybrid->setProperty("hmvm", HMVM_Hybrid);

    QMenu *mnuView = menuBar()->addMenu(tr("&View"));
    mnuView->addAction(m->actViewModePeaks);
    mnuView->addAction(m->actViewModeLandscape);
    mnuView->addAction(m->actViewModeIsobars);
    mnuView->addAction(m->actViewModeHybrid);

    QMenu *mnuWindows = menuBar()->addMenu(tr("&Windows"));
    mnuWindows->addAction(m->dckPeakGenerating->toggleViewAction());
    mnuWindows->addAction(m->dckExtrapolation->toggleViewAction());
    mnuWindows->addAction(m->dckContouring->toggleViewAction());

    typedef QAction A;
    typedef QActionGroup G;
    typedef HeightMapWindow W;

    connect(m->actNewFile,          &A::triggered,  this,       &W::newFile);
    connect(m->actExportLs,         &A::triggered,  this,       &W::exportLandscape);
    connect(m->actExportPk,         &A::triggered,  this,       &W::exportPeaks);
    connect(m->actExit,             &A::triggered,  this,       &W::close);
    connect(m->actGenLs,            &A::triggered,  this,       &W::generateLandscape);
    connect(m->actBuildLs,          &A::triggered,  this,       &W::extrapolatePeaks);
    connect(m->actCalcContours,     &A::triggered,  this,       &W::calculateContours);
    connect(m->actHmSettings,       &A::triggered,  this,       &W::editPeakSettings);
    connect(m->actExtrapolSettings, &A::triggered,  this,       &W::editExtrapolationSettings);
    connect(m->actContourSettings,  &A::triggered,  this,       &W::editContouringSettings);
    connect(agpViewMode,            &G::triggered,  this,       &W::setViewMode);
}

void HeightMapWindow::createStatusBar()
{
    QWidget *procBarShell = new QWidget(this);
    QBoxLayout *procBarLayout = new QHBoxLayout(procBarShell);
    procBarLayout->setMargin(0);
    procBarLayout->addWidget(m->prgProcess);
    procBarShell->setMaximumHeight(m->lblProcess->sizeHint().height());

    m->prgProcess->hide();

    statusBar()->addWidget(m->lblState, 6);
    statusBar()->addWidget(m->lblLandscape, 3);
    statusBar()->addWidget(m->lblLevels, 3);
    statusBar()->addWidget(m->lblProcess, 8);
    statusBar()->addWidget(procBarShell, 12);
    statusBar()->addWidget(m->lblPeaks, 4);
    statusBar()->addWidget(m->lblIsobars, 6);
}


} // namespace HeightMap
