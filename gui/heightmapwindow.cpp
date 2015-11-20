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
#include <QMessageBox>
#include <QCloseEvent>
#include "heightmapapplication.h"
#include "heightmaplogic.h"
#include "terrain.h"
#include "preferences.h"
#include "preferencescontroller.h"
#include "extrapolationdata.h"
#include "extrapolationfactory.h"
#include "iohandlers"
#include "commands"
#include "guielements"

#include "heightmapwindow.h"


namespace HeightMap {


namespace {
const char *HM_FILE_EXT = "hmt";
const char *LS_TEXT_FILE_EXT = "hmlst";
const char *PK_TEXT_FILE_EXT = "hmpkt";
}


struct HeightMapWindowImplementation
{
    HeightMapWindowImplementation();

    void adjustFileDialog(QFileDialog *);
    void provideExtrapolationWidgets(ExtrapolationOptionsDialog *);

    void loadTerrain(const QString &filename);
    void saveTerrain(const QString &filename);
    void saveLandscape(const QString &filename);
    void savePeaks(const QString &filename);

    void displayHeightMapImage();
    void resetStatusBar();

    void setLogicActionsEnabled(bool);
    void setWindowActionsEnabled(bool);
    void setWidgetsEnabled(bool);

    QString strippedName(const QString &fullFilename) const;

    ~HeightMapWindowImplementation();

    HeightMapLogic *logic;
    QString currentFilename;

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

    QAction *actNew;
    QAction *actOpen;
    QAction *actSave;
    QAction *actSaveAs;
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
    int historyPivotIdx;

    HeightMapViewMode hmvm;

    bool processing;

private:
    DISABLE_COPY(HeightMapWindowImplementation)
    DISABLE_MOVE(HeightMapWindowImplementation)
};


HeightMapWindowImplementation::HeightMapWindowImplementation()
    : logic(),
      currentFilename(),
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
      actNew(nullptr),
      actOpen(nullptr),
      actSave(nullptr),
      actSaveAs(nullptr),
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
      historyPivotIdx(-1),
      hmvm(HMVM_Hybrid),
      processing(false) { }

void HeightMapWindowImplementation::adjustFileDialog(QFileDialog *dialog)
{
    dialog->setOptions(QFileDialog::DontResolveSymlinks);
    dialog->setDirectory(QDir::home());
    dialog->setFileMode(QFileDialog::AnyFile);
    dialog->setViewMode(QFileDialog::Detail);
}

void HeightMapWindowImplementation::provideExtrapolationWidgets(ExtrapolationOptionsDialog *dialog)
{
    QStringList xNames = hmApp->extrapolatorKeys();
    foreach (QString name, xNames) {
        if (ExtrapolationFactory *f = hmApp->extrapolationFactory(name))
            dialog->addExtrapolationWidget(f);
    }
}

void HeightMapWindowImplementation::loadTerrain(const QString &filename)
{
    HeightMapBinaryIOHandler ioHandler;
    Terrain *t = ioHandler.read(filename);
    hmApp->setPreferences(ioHandler.preferences());
    hmApp->setXData(hmApp->preferences().extrapolatorName(), ioHandler.xData());

    logic->openTerrain(t);
}

void HeightMapWindowImplementation::saveTerrain(const QString &filename)
{
    HeightMapBinaryIOHandler ioHandler;
    ioHandler.setPreferences(logic->preferences());
    ioHandler.setXData(logic->xData());
    ioHandler.write(logic->terrain(), filename);
}

void HeightMapWindowImplementation::saveLandscape(const QString &filename)
{
    LandscapeTextIOHandler ioHandler;
    ioHandler.write(logic->terrain(), filename);
}

void HeightMapWindowImplementation::savePeaks(const QString &filename)
{
    PeaksTextIOHandler ioHandler;
    ioHandler.write(logic->terrain(), filename);
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

void HeightMapWindowImplementation::setLogicActionsEnabled(bool enabled)
{
    actSave->setEnabled(enabled);
    actSaveAs->setEnabled(enabled);
    actExportLs->setEnabled(enabled);
    actExportPk->setEnabled(enabled);

    actUndo->setEnabled(enabled && uskCommands->canUndo());
    actRedo->setEnabled(enabled && uskCommands->canRedo());

    actGenLs->setEnabled(enabled);
    actBuildLs->setEnabled(enabled);
    actCalcContours->setEnabled(enabled);
    actHmSettings->setEnabled(enabled);
    actExtrapolSettings->setEnabled(enabled);
    actContourSettings->setEnabled(enabled);
}

void HeightMapWindowImplementation::setWindowActionsEnabled(bool enabled)
{
    actNew->setEnabled(enabled);
    actOpen->setEnabled(enabled);

    actExit->setEnabled(enabled);

    actViewModePeaks->setEnabled(enabled);
    actViewModeLandscape->setEnabled(enabled);
    actViewModeIsobars->setEnabled(enabled);
    actViewModeHybrid->setEnabled(enabled);
}

void HeightMapWindowImplementation::setWidgetsEnabled(bool enabled)
{
    wgtPeakGenerating->setEnabled(enabled);
    wgtExtrapolation->setEnabled(enabled);
    wgtContouring->setEnabled(enabled);
}

QString HeightMapWindowImplementation::strippedName(const QString &fullFilename) const
{
    QString filename = QFileInfo(fullFilename).fileName();
    return filename.left(filename.indexOf(QChar('.')));
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
    connect(m->logic,   &L::estimatedTimeCalculated,    this,   &W::onEstimatedTimeCalculated);
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


void HeightMapWindow::closeEvent(QCloseEvent *e)
{
    if (m->processing) {
        QMessageBox msg(this);
        msg.setText(tr("Can not close."));
        msg.setInformativeText(tr("Background operations are being performed. Please wait."));
        msg.setIcon(QMessageBox::Information);
        msg.exec();

        e->ignore();
    } else if (!handleModified()) {
        e->ignore();
    } else {
        e->accept();
    }
}


void HeightMapWindow::newFile()
{
    if (!handleModified())
        return;

    TerrainOptionsDialog dialog(this);
    dialog.setWindowTitle(tr("New terrain"));
    dialog.setPreferences(m->logic->preferences());

    if (!dialog.exec())
        return;

    hmApp->setPreferences(dialog.preferences());
    m->logic->newTerrain();
}

void HeightMapWindow::openFile()
{
    if (!handleModified())
        return;

    QFileDialog dialog(this);
    m->adjustFileDialog(&dialog);
    dialog.setWindowTitle(tr("Open terrain"));
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilter(tr("HeightMap terrain files (*.%1)").arg(HM_FILE_EXT));

    if (!dialog.exec())
        return;

    QString filename(dialog.selectedFiles().value(0));
    if (!filename.isEmpty()) {
        loadTerrain(filename);
    }
}

bool HeightMapWindow::saveFile()
{
    if (!m->currentFilename.isEmpty()) {
        saveTerrain(m->currentFilename);
        return true;
    } else {
        return saveAsFile();
    }
}

bool HeightMapWindow::saveAsFile()
{
    QFileDialog dialog(this);
    m->adjustFileDialog(&dialog);
    dialog.setWindowTitle(tr("Save terrain"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter(tr("HeightMap terrain files (*.%1)").arg(HM_FILE_EXT));

    if (!dialog.exec())
        return false;

    QString filename(dialog.selectedFiles().value(0));
    if (!filename.isEmpty()) {
        saveTerrain(filename);
        return true;
    }

    return false;
}

void HeightMapWindow::exportLandscape()
{
    QFileDialog dialog(this);
    m->adjustFileDialog(&dialog);
    dialog.setWindowTitle(tr("Export landscape"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter(tr("HeightMap landscape text files (*.%1)").arg(LS_TEXT_FILE_EXT));

    if (!dialog.exec())
        return;

    QString filename(dialog.selectedFiles().value(0));
    if (!filename.isEmpty())
        m->saveLandscape(filename);
}

void HeightMapWindow::exportPeaks()
{
    QFileDialog dialog(this);
    m->adjustFileDialog(&dialog);
    dialog.setWindowTitle(tr("Export peaks"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter(tr("HeightMap peak text files (*.%1)").arg(PK_TEXT_FILE_EXT));

    if (!dialog.exec())
        return;

    QString filename(dialog.selectedFiles().value(0));
    if (!filename.isEmpty())
        m->savePeaks(filename);
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

    m->setLogicActionsEnabled(true);
    m->setWidgetsEnabled(true);

    specifyCurrentFilename(QString());
}

void HeightMapWindow::onProcessStarted()
{
    m->setWindowActionsEnabled(false);
    m->setLogicActionsEnabled(false);
    m->setWidgetsEnabled(false);

    m->lblState->setText(tr("Processing..."));
    m->prgProcess->setValue(0);
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

    m->setWindowActionsEnabled(true);
    m->setLogicActionsEnabled(true);
    m->setWidgetsEnabled(true);
}

void HeightMapWindow::onEstimatedTimeCalculated(int time)
{
    m->prgProcess->setMaximum(time);
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

void HeightMapWindow::adjustHistoryIndex(int historyIndex)
{
    setWindowModified(historyIndex != m->historyPivotIdx);
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
    m->actNew = new QAction(this);
    m->actNew->setText(tr("&New terrain..."));
    m->actNew->setShortcut(tr("Ctrl+N"));

    m->actOpen = new QAction(this);
    m->actOpen->setText(tr("&Open terrain..."));
    m->actOpen->setShortcut(tr("Ctrl+O"));

    m->actSave = new QAction(this);
    m->actSave->setText(tr("&Save terrain"));
    m->actSave->setShortcut(tr("Ctrl+S"));

    m->actSaveAs = new QAction(this);
    m->actSaveAs->setText(tr("&Save terrain as..."));
    m->actSaveAs->setShortcut(tr("Ctrl+Shift+S"));

    m->actExportLs = new QAction(this);
    m->actExportLs->setText(tr("Export landscape..."));

    m->actExportPk = new QAction(this);
    m->actExportPk->setText(tr("Export peaks..."));

    m->actExit = new QAction(this);
    m->actExit->setText(tr("E&xit"));
    m->actExit->setShortcut(tr("Alt+X"));

    QMenu *mnuFile = menuBar()->addMenu(tr("&File"));
    mnuFile->addAction(m->actNew);
    mnuFile->addAction(m->actOpen);
    mnuFile->addAction(m->actSave);
    mnuFile->addAction(m->actSaveAs);
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

    QMenu *mnuWindows = menuBar()->addMenu(tr("&Windows"));
    mnuWindows->addAction(m->dckPeakGenerating->toggleViewAction());
    mnuWindows->addAction(m->dckExtrapolation->toggleViewAction());
    mnuWindows->addAction(m->dckContouring->toggleViewAction());

    typedef QAction A;
    typedef QActionGroup G;
    typedef QUndoStack U;
    typedef HeightMapWindow W;

    connect(m->uskCommands,         &U::indexChanged,   this,   &W::adjustHistoryIndex);

    connect(m->actNew,              &A::triggered,      this,   &W::newFile);
    connect(m->actOpen,             &A::triggered,      this,   &W::openFile);
    connect(m->actSave,             &A::triggered,      this,   &W::saveFile);
    connect(m->actSaveAs,           &A::triggered,      this,   &W::saveAsFile);
    connect(m->actExportLs,         &A::triggered,      this,   &W::exportLandscape);
    connect(m->actExportPk,         &A::triggered,      this,   &W::exportPeaks);
    connect(m->actExit,             &A::triggered,      this,   &W::close);
    connect(m->actGenLs,            &A::triggered,      this,   &W::generateLandscape);
    connect(m->actBuildLs,          &A::triggered,      this,   &W::extrapolatePeaks);
    connect(m->actCalcContours,     &A::triggered,      this,   &W::calculateContours);
    connect(m->actHmSettings,       &A::triggered,      this,   &W::editPeakSettings);
    connect(m->actExtrapolSettings, &A::triggered,      this,   &W::editExtrapolationSettings);
    connect(m->actContourSettings,  &A::triggered,      this,   &W::editContouringSettings);
    connect(agpViewMode,            &G::triggered,      this,   &W::setViewMode);

    m->setWindowActionsEnabled(true);
    m->setLogicActionsEnabled(false);
    m->setWidgetsEnabled(false);
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


void HeightMapWindow::loadTerrain(const QString &filename)
{
    m->loadTerrain(filename);
    specifyCurrentFilename(filename);
    statusBar()->showMessage(tr("Terrain loaded"), 2000);
}

void HeightMapWindow::saveTerrain(const QString &filename)
{
    m->saveTerrain(filename);
    specifyCurrentFilename(filename);
    statusBar()->showMessage(tr("Terrain saved"), 2000);
}


bool HeightMapWindow::handleModified()
{
    if (!isWindowModified())
        return true;

    QMessageBox msg(this);
    msg.setWindowTitle(hmApp->applicationName());
    msg.setIcon(QMessageBox::Warning);
    msg.setText(tr("The document has been modified.\n"
                   "Do you want to save changes?"));
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    switch (msg.exec()) {
    case QMessageBox::Yes:
        return saveFile();
    case QMessageBox::No:
        return true;
    default:
        return false;
    }
}


void HeightMapWindow::specifyCurrentFilename(const QString &filename)
{
    m->currentFilename = filename;
    m->historyPivotIdx = m->uskCommands->index();
    setWindowModified(false);

    QString shownName = tr("Untitled");
    if (!m->currentFilename.isEmpty()) {
        shownName = m->strippedName(filename);
    }

    setWindowTitle(tr("%1 - %2[*]").arg(hmApp->applicationName())
                                   .arg(shownName));
}


} // namespace HeightMap
