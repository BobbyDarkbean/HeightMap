#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QSlider>
#include <QGroupBox>
#include <QBoxLayout>
#include <QGridLayout>
#include "preferences.h"

#include "heightmapsettingsdialog.h"


namespace HeightMap {


struct HeightMapSettingsDialogImplementation
{
    HeightMapSettingsDialogImplementation();

    void adjustControls();
    void adjustLayout(QDialog *master);

    ~HeightMapSettingsDialogImplementation();

    QFrame *frmMain;

    QLabel *lblLandscapeWidth;
    QSpinBox *spnLandscapeWidth;
    QLabel *lblLandscapeHeight;
    QSpinBox *spnLandscapeHeight;
    QLabel *lblLandscapeBase;
    QSpinBox *spnLandscapeBase;

    QGroupBox *grpGenOpts;
    QLabel *lblMinPeak;
    QSlider *sldMinPeak;
    QSpinBox *spnMinPeak;
    QLabel *lblMaxPeak;
    QSlider *sldMaxPeak;
    QSpinBox *spnMaxPeak;
    QLabel *lblPeakCount;
    QSlider *sldPeakCount;
    QSpinBox *spnPeakCount;

    QPushButton *btnOk;
    QPushButton *btnCancel;

    Preferences prefs;

private:
    DISABLE_COPY(HeightMapSettingsDialogImplementation)
    DISABLE_MOVE(HeightMapSettingsDialogImplementation)
};


HeightMapSettingsDialogImplementation::HeightMapSettingsDialogImplementation()
    : frmMain(new QFrame),
      lblLandscapeWidth(new QLabel),
      spnLandscapeWidth(new QSpinBox),
      lblLandscapeHeight(new QLabel),
      spnLandscapeHeight(new QSpinBox),
      lblLandscapeBase(new QLabel),
      spnLandscapeBase(new QSpinBox),
      grpGenOpts(new QGroupBox),
      lblMinPeak(new QLabel),
      sldMinPeak(new QSlider),
      spnMinPeak(new QSpinBox),
      lblMaxPeak(new QLabel),
      sldMaxPeak(new QSlider),
      spnMaxPeak(new QSpinBox),
      lblPeakCount(new QLabel),
      sldPeakCount(new QSlider),
      spnPeakCount(new QSpinBox),
      btnOk(new QPushButton),
      btnCancel(new QPushButton),
      prefs() { }

void HeightMapSettingsDialogImplementation::adjustControls()
{
    // Labels
    lblLandscapeWidth->setText(QObject::tr("Width:"));
    lblLandscapeHeight->setText(QObject::tr("Height:"));
    lblLandscapeBase->setText(QObject::tr("Base:"));
    lblMinPeak->setText(QObject::tr("Min. peak:"));
    lblMaxPeak->setText(QObject::tr("Max. peak:"));
    lblPeakCount->setText(QObject::tr("Peak count:"));

    lblLandscapeWidth->setBuddy(spnLandscapeWidth);
    lblLandscapeHeight->setBuddy(spnLandscapeHeight);
    lblLandscapeBase->setBuddy(spnLandscapeBase);

    // Spin-boxes
    spnLandscapeWidth->setRange(Preferences::MinLandscapeDim, Preferences::MaxLandscapeDim);
    spnLandscapeHeight->setRange(Preferences::MinLandscapeDim, Preferences::MaxLandscapeDim);
    spnLandscapeBase->setRange(Preferences::MinLevel, Preferences::MaxLevel);
    spnMinPeak->setRange(Preferences::MinLevel, Preferences::MaxLevel);
    spnMaxPeak->setRange(Preferences::MinLevel, Preferences::MaxLevel);
    spnPeakCount->setRange(1, 8192);

    spnLandscapeWidth->setSingleStep(1);
    spnLandscapeHeight->setSingleStep(1);
    spnLandscapeBase->setSingleStep(1);
    spnMinPeak->setSingleStep(1);
    spnMaxPeak->setSingleStep(1);
    spnPeakCount->setSingleStep(1);

    spnLandscapeWidth->setAccelerated(true);
    spnLandscapeHeight->setAccelerated(true);
    spnLandscapeBase->setAccelerated(true);
    spnMinPeak->setAccelerated(true);
    spnMaxPeak->setAccelerated(true);
    spnPeakCount->setAccelerated(true);

    spnLandscapeWidth->setAlignment(Qt::AlignRight);
    spnLandscapeHeight->setAlignment(Qt::AlignRight);
    spnLandscapeBase->setAlignment(Qt::AlignRight);
    spnMinPeak->setAlignment(Qt::AlignRight);
    spnMaxPeak->setAlignment(Qt::AlignRight);
    spnPeakCount->setAlignment(Qt::AlignRight);

    // Sliders
    sldMinPeak->setOrientation(Qt::Horizontal);
    sldMaxPeak->setOrientation(Qt::Horizontal);
    sldPeakCount->setOrientation(Qt::Horizontal);

    sldMinPeak->setRange(Preferences::MinLevel, Preferences::MaxLevel);
    sldMaxPeak->setRange(Preferences::MinLevel, Preferences::MaxLevel);
    sldPeakCount->setRange(1, 8192);

    sldMinPeak->setSingleStep(1);
    sldMaxPeak->setSingleStep(1);
    sldPeakCount->setSingleStep(128);

    sldMinPeak->setTickInterval(16);
    sldMaxPeak->setTickInterval(16);
    sldPeakCount->setTickInterval(1024);

    sldMinPeak->setTickPosition(QSlider::TicksBelow);
    sldMaxPeak->setTickPosition(QSlider::TicksBelow);
    sldPeakCount->setTickPosition(QSlider::TicksBelow);

    // Buttons
    btnOk->setText(QObject::tr("OK"));
    btnCancel->setText(QObject::tr("Cancel"));

    QObject::connect(spnMinPeak, SIGNAL(valueChanged(int)), sldMinPeak, SLOT(setValue(int)));
    QObject::connect(spnMaxPeak, SIGNAL(valueChanged(int)), sldMaxPeak, SLOT(setValue(int)));
    QObject::connect(spnPeakCount, SIGNAL(valueChanged(int)), sldPeakCount, SLOT(setValue(int)));

    QObject::connect(sldMinPeak, SIGNAL(valueChanged(int)), spnMinPeak, SLOT(setValue(int)));
    QObject::connect(sldMaxPeak, SIGNAL(valueChanged(int)), spnMaxPeak, SLOT(setValue(int)));
    QObject::connect(sldPeakCount, SIGNAL(valueChanged(int)), spnPeakCount, SLOT(setValue(int)));
}

void HeightMapSettingsDialogImplementation::adjustLayout(QDialog *master)
{
    QBoxLayout *lytLandscapeDim = new QHBoxLayout;
    lytLandscapeDim->addStretch();
    lytLandscapeDim->addWidget(lblLandscapeWidth);
    lytLandscapeDim->addWidget(spnLandscapeWidth);
    lytLandscapeDim->addStretch();
    lytLandscapeDim->addWidget(lblLandscapeHeight);
    lytLandscapeDim->addWidget(spnLandscapeHeight);
    lytLandscapeDim->addStretch();

    QBoxLayout *lytLandscapeBase = new QHBoxLayout;
    lytLandscapeBase->addStretch();
    lytLandscapeBase->addWidget(lblLandscapeBase);
    lytLandscapeBase->addWidget(spnLandscapeBase);
    lytLandscapeBase->addStretch();

    QBoxLayout *lytLandscape = new QVBoxLayout;
    lytLandscape->addLayout(lytLandscapeDim);
    lytLandscape->addLayout(lytLandscapeBase);

    QGridLayout *lytGenOpts = new QGridLayout(grpGenOpts);
    lytGenOpts->addWidget(lblMinPeak, 0, 0);
    lytGenOpts->addWidget(sldMinPeak, 0, 1);
    lytGenOpts->addWidget(spnMinPeak, 0, 2);
    lytGenOpts->addWidget(lblMaxPeak, 1, 0);
    lytGenOpts->addWidget(sldMaxPeak, 1, 1);
    lytGenOpts->addWidget(spnMaxPeak, 1, 2);
    lytGenOpts->addWidget(lblPeakCount, 2, 0);
    lytGenOpts->addWidget(sldPeakCount, 2, 1);
    lytGenOpts->addWidget(spnPeakCount, 2, 2);

    QBoxLayout *lytButtons = new QHBoxLayout;
    lytButtons->addStretch();
    lytButtons->addWidget(btnOk);
    lytButtons->addWidget(btnCancel);

    QBoxLayout *lytMain = new QVBoxLayout(frmMain);
    lytMain->addLayout(lytLandscape);
    lytMain->addWidget(grpGenOpts);
    lytMain->addLayout(lytButtons);

    QBoxLayout *lytShell = new QVBoxLayout(master);
    lytShell->addWidget(frmMain);

    QObject::connect(btnOk, SIGNAL(clicked()), master, SLOT(accept()));
    QObject::connect(btnCancel, SIGNAL(clicked()), master, SLOT(reject()));
}

HeightMapSettingsDialogImplementation::~HeightMapSettingsDialogImplementation() { }


HeightMapSettingsDialog::HeightMapSettingsDialog(QWidget *parent) :
    QDialog(parent),
    m(new HeightMapSettingsDialogImplementation)
{
    m->adjustControls();
    m->adjustLayout(this);
}


const Preferences &HeightMapSettingsDialog::preferences() const
{ return m->prefs; }

void HeightMapSettingsDialog::setPreferences(const Preferences &prefs)
{
    m->prefs = prefs;
    m->spnLandscapeWidth->setValue(m->prefs.landscapeWidth());
    m->spnLandscapeHeight->setValue(m->prefs.landscapeHeight());
    m->spnLandscapeBase->setValue(m->prefs.landscapeBase());
    m->spnMinPeak->setValue(m->prefs.minPeak());
    m->spnMaxPeak->setValue(m->prefs.maxPeak());
    m->spnPeakCount->setValue(m->prefs.peakCount());
}


HeightMapSettingsDialog::~HeightMapSettingsDialog()
{
    delete m;
}


void HeightMapSettingsDialog::done(int r)
{
    if (r == Accepted) {
        m->prefs.setLandscapeWidth(m->spnLandscapeWidth->value());
        m->prefs.setLandscapeHeight(m->spnLandscapeHeight->value());
        m->prefs.setLandscapeBase(m->spnLandscapeBase->value());
        m->prefs.setMinPeak(qMin(m->spnMinPeak->value(), m->spnMaxPeak->value()));
        m->prefs.setMaxPeak(qMax(m->spnMinPeak->value(), m->spnMaxPeak->value()));
        m->prefs.setPeakCount(m->spnPeakCount->value());
    }

    QDialog::done(r);
}


} // namespace HeightMap
