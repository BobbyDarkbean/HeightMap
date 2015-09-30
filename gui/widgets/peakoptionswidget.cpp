#include <QLabel>
#include <QSpinBox>
#include <QSlider>
#include <QGroupBox>
#include <QLayout>
#include "../preferences.h"

#include "peakoptionswidget.h"


namespace HeightMap {


struct PeakOptionsWidgetImplementation
{
    PeakOptionsWidgetImplementation();

    void adjustControls(PeakOptionsWidget *master);
    void adjustLayout(QWidget *master);

    ~PeakOptionsWidgetImplementation();

    QGroupBox *grpPeakOpts;
    QLabel *lblMinPeak;
    QSlider *sldMinPeak;
    QSpinBox *spnMinPeak;
    QLabel *lblMaxPeak;
    QSlider *sldMaxPeak;
    QSpinBox *spnMaxPeak;
    QLabel *lblPeakCount;
    QSlider *sldPeakCount;
    QSpinBox *spnPeakCount;

private:
    DISABLE_COPY(PeakOptionsWidgetImplementation)
    DISABLE_MOVE(PeakOptionsWidgetImplementation)
};


PeakOptionsWidgetImplementation::PeakOptionsWidgetImplementation()
    : grpPeakOpts(new QGroupBox),
      lblMinPeak(new QLabel),
      sldMinPeak(new QSlider),
      spnMinPeak(new QSpinBox),
      lblMaxPeak(new QLabel),
      sldMaxPeak(new QSlider),
      spnMaxPeak(new QSpinBox),
      lblPeakCount(new QLabel),
      sldPeakCount(new QSlider),
      spnPeakCount(new QSpinBox) { }

void PeakOptionsWidgetImplementation::adjustControls(PeakOptionsWidget *master)
{
    // Labels
    lblMinPeak->setText(PeakOptionsWidget::tr("Min. peak:"));
    lblMaxPeak->setText(PeakOptionsWidget::tr("Max. peak:"));
    lblPeakCount->setText(PeakOptionsWidget::tr("Peak count:"));

    // Spin-boxes
    spnMinPeak->setRange(Preferences::MinLevel, Preferences::MaxLevel);
    spnMaxPeak->setRange(Preferences::MinLevel, Preferences::MaxLevel);
    spnPeakCount->setRange(1, 8192);

    spnMinPeak->setSingleStep(1);
    spnMaxPeak->setSingleStep(1);
    spnPeakCount->setSingleStep(1);

    spnMinPeak->setAccelerated(true);
    spnMaxPeak->setAccelerated(true);
    spnPeakCount->setAccelerated(true);

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

    // Group-box
    grpPeakOpts->setTitle(PeakOptionsWidget::tr("Peak generating options"));

    QObject::connect(spnMinPeak, SIGNAL(valueChanged(int)), sldMinPeak, SLOT(setValue(int)));
    QObject::connect(spnMaxPeak, SIGNAL(valueChanged(int)), sldMaxPeak, SLOT(setValue(int)));
    QObject::connect(spnPeakCount, SIGNAL(valueChanged(int)), sldPeakCount, SLOT(setValue(int)));

    QObject::connect(sldMinPeak, SIGNAL(valueChanged(int)), spnMinPeak, SLOT(setValue(int)));
    QObject::connect(sldMaxPeak, SIGNAL(valueChanged(int)), spnMaxPeak, SLOT(setValue(int)));
    QObject::connect(sldPeakCount, SIGNAL(valueChanged(int)), spnPeakCount, SLOT(setValue(int)));

    QObject::connect(spnMinPeak, SIGNAL(valueChanged(int)), master, SIGNAL(minPeakChanged(int)));
    QObject::connect(spnMaxPeak, SIGNAL(valueChanged(int)), master, SIGNAL(maxPeakChanged(int)));
    QObject::connect(spnPeakCount, SIGNAL(valueChanged(int)), master, SIGNAL(peakCountChanged(int)));
}

void PeakOptionsWidgetImplementation::adjustLayout(QWidget *master)
{
    QGridLayout *lytGenOpts = new QGridLayout(grpPeakOpts);
    lytGenOpts->addWidget(lblMinPeak, 0, 0);
    lytGenOpts->addWidget(sldMinPeak, 0, 1);
    lytGenOpts->addWidget(spnMinPeak, 0, 2);
    lytGenOpts->setRowMinimumHeight(1, 12);
    lytGenOpts->addWidget(lblMaxPeak, 2, 0);
    lytGenOpts->addWidget(sldMaxPeak, 2, 1);
    lytGenOpts->addWidget(spnMaxPeak, 2, 2);
    lytGenOpts->setRowMinimumHeight(3, 12);
    lytGenOpts->addWidget(lblPeakCount, 4, 0);
    lytGenOpts->addWidget(sldPeakCount, 4, 1);
    lytGenOpts->addWidget(spnPeakCount, 4, 2);

    QBoxLayout *lytMain = new QVBoxLayout(master);
    lytMain->addWidget(grpPeakOpts);
    lytMain->addStretch();
}

PeakOptionsWidgetImplementation::~PeakOptionsWidgetImplementation() { }


PeakOptionsWidget::PeakOptionsWidget(QWidget *parent)
    : QWidget(parent),
      m(new PeakOptionsWidgetImplementation)
{
    m->adjustControls(this);
    m->adjustLayout(this);
}


QSize PeakOptionsWidget::sizeHint() const
{ return QSize(540, 120); }


int PeakOptionsWidget::minPeak() const
{ return qMin(m->spnMinPeak->value(), m->spnMaxPeak->value()); }

int PeakOptionsWidget::maxPeak() const
{ return qMax(m->spnMinPeak->value(), m->spnMaxPeak->value()); }

void PeakOptionsWidget::setRange(int minValue, int maxValue)
{
    m->spnMinPeak->setValue(qMin(minValue, maxValue));
    m->spnMaxPeak->setValue(qMax(minValue, maxValue));
}

unsigned int PeakOptionsWidget::peakCount() const
{ return static_cast<int>(m->spnPeakCount->value()); }

void PeakOptionsWidget::setPeakCount(unsigned int value)
{ m->spnPeakCount->setValue(static_cast<int>(value)); }


PeakOptionsWidget::~PeakOptionsWidget()
{
    delete m;
}


} // namespace HeightMap
