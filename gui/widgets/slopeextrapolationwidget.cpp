#include <QLabel>
#include <QSpinBox>
#include <QGridLayout>
#include "extrapolator.h"
#include "../preferences.h"

#include "slopeextrapolationwidget.h"


namespace HeightMap {


struct SlopeExtrapolationWidgetImplementation
{
    SlopeExtrapolationWidgetImplementation();

    void adjustControls();
    void adjustLayout(SlopeExtrapolationWidget *master);
    void adjustValues();

    ~SlopeExtrapolationWidgetImplementation();

    QLabel *lblBaseLevel;
    QDoubleSpinBox *spnBaseLevel;

    QLabel *lblSlope;
    QDoubleSpinBox *spnSlope;

    double bl;
    double sr;
    bool infl;

    SlopeExtrapolator *x;

private:
    DISABLE_COPY(SlopeExtrapolationWidgetImplementation)
    DISABLE_MOVE(SlopeExtrapolationWidgetImplementation)
};


SlopeExtrapolationWidgetImplementation::SlopeExtrapolationWidgetImplementation()
    : lblBaseLevel(new QLabel),
      spnBaseLevel(new QDoubleSpinBox),
      lblSlope(new QLabel),
      spnSlope(new QDoubleSpinBox),
      bl(static_cast<double>(Preferences::MinLevel)),
      sr(1.0),
      infl(false),
      x(nullptr) { }

void SlopeExtrapolationWidgetImplementation::adjustControls()
{
    // Labels
    lblBaseLevel->setText(SlopeExtrapolationWidget::tr("Landscape base level:"));
    lblSlope->setText(SlopeExtrapolationWidget::tr("Slope ratio:"));

    lblBaseLevel->setBuddy(spnBaseLevel);
    lblSlope->setBuddy(spnSlope);

    // Spin-boxes
    spnBaseLevel->setDecimals(2);
    spnBaseLevel->setRange(static_cast<double>(Preferences::MinLevel),
                           static_cast<double>(Preferences::MaxLevel));
    spnBaseLevel->setSingleStep(0.01);
    spnBaseLevel->setAccelerated(true);
    spnBaseLevel->setAlignment(Qt::AlignRight);

    spnSlope->setDecimals(2);
    spnSlope->setRange(1.0, 32.0);
    spnSlope->setSingleStep(0.01);
    spnSlope->setAccelerated(true);
    spnSlope->setAlignment(Qt::AlignRight);
}

void SlopeExtrapolationWidgetImplementation::adjustLayout(SlopeExtrapolationWidget *master)
{
    QGridLayout *lytContent = new QGridLayout;
    lytContent->setColumnStretch(0, 1);
    lytContent->addWidget(lblBaseLevel, 0, 1);
    lytContent->setColumnStretch(2, 1);
    lytContent->addWidget(spnBaseLevel, 0, 3);
    lytContent->setColumnStretch(4, 1);
    lytContent->setRowMinimumHeight(1, 12);
    lytContent->addWidget(lblSlope, 2, 1);
    lytContent->addWidget(spnSlope, 2, 3);

    QBoxLayout *lytMain = new QVBoxLayout(master);
    lytMain->addLayout(lytContent);
    lytMain->addStretch();

    SlopeExtrapolationWidget::connect(
                spnBaseLevel, SIGNAL(valueChanged(double)), master, SLOT(setBaseLevel(double)));
    SlopeExtrapolationWidget::connect(
                spnSlope, SIGNAL(valueChanged(double)), master, SLOT(setSlopeRatio(double)));
}

void SlopeExtrapolationWidgetImplementation::adjustValues()
{
    spnBaseLevel->setValue(x->baseLevel());
    spnSlope->setValue(x->slopeRatio());
}

SlopeExtrapolationWidgetImplementation::~SlopeExtrapolationWidgetImplementation() { }


SlopeExtrapolationWidget::SlopeExtrapolationWidget(QWidget *parent)
    : AbstractExtrapolationWidget(parent),
      m(new SlopeExtrapolationWidgetImplementation)
{
    m->adjustControls();
    m->adjustLayout(this);
}


Extrapolator *SlopeExtrapolationWidget::extrapolator() const
{ return m->x; }

void SlopeExtrapolationWidget::bindExtrapolator(SlopeExtrapolator *sx)
{
    m->x = sx;
    refreshData();
}

bool SlopeExtrapolationWidget::directInfluence() const
{ return m->infl; }

void SlopeExtrapolationWidget::setDirectInfluence(bool infl)
{ m->infl = infl; }

void SlopeExtrapolationWidget::refreshData()
{ m->adjustValues(); }

void SlopeExtrapolationWidget::acceptSettings()
{
    m->x->setBaseLevel(m->bl);
    m->x->setSlopeRatio(m->sr);
}


SlopeExtrapolationWidget::~SlopeExtrapolationWidget()
{
    delete m;
}


void SlopeExtrapolationWidget::setBaseLevel(double bl)
{
    m->bl = bl;
    if (m->infl)
        m->x->setBaseLevel(m->bl);
}

void SlopeExtrapolationWidget::setSlopeRatio(double sr)
{
    m->sr = sr;
    if (m->infl)
        m->x->setSlopeRatio(m->sr);
}


} // namespace HeightMap
