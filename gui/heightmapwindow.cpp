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

    QMenu *mnuLandscape = menuBar()->addMenu(tr("Landscape"));
    mnuLandscape->addAction(actGenLs);

    statusBar()->addWidget(m->stateLabel, 1);

    connect(worker, SIGNAL(processStarted()), this, SLOT(onProcessStarted()));
    connect(worker, SIGNAL(processFinished()), this, SLOT(onProcessFinished()));

    connect(&m->procThread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    m->procThread.start();
}

HeightMapWindow::~HeightMapWindow()
{
    delete m;
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
    m->engr.drawLandscape(m->landscape, &hmImg, ImageFactor);

    m->hmImgLabel->setPixmap(QPixmap::fromImage(hmImg));
}


} // namespace HeightMap
