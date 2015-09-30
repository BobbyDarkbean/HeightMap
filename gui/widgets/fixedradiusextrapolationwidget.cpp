#include <QLabel>
#include <QSpinBox>
#include <QGridLayout>
#include "extrapolator.h"
#include "../preferences.h"

#include "fixedradiusextrapolationwidget.h"


namespace HeightMap {


struct FixedRadiusExtrapolationWidgetImplementation
{
    FixedRadiusExtrapolationWidgetImplementation();

    void adjustControls();
    void adjustLayout(FixedRadiusExtrapolationWidget *master);
    void adjustValues();

    ~FixedRadiusExtrapolationWidgetImplementation();

    QLabel *lblBaseLevel;
    QDoubleSpinBox *spnBaseLevel;

    QLabel *lblFixedRadius;
    QSpinBox *spnFixedRadius;

    double bl;
    int fr;
    bool infl;

    FixedRadiusExtrapolator *x;

private:
    DISABLE_COPY(FixedRadiusExtrapolationWidgetImplementation)
    DISABLE_MOVE(FixedRadiusExtrapolationWidgetImplementation)
};


FixedRadiusExtrapolationWidgetImplementation::FixedRadiusExtrapolationWidgetImplementation()
    : lblBaseLevel(new QLabel),
      spnBaseLevel(new QDoubleSpinBox),
      lblFixedRadius(new QLabel),
      spnFixedRadius(new QSpinBox),
      bl(static_cast<double>(Preferences::MinLevel)),
      fr(16),
      infl(false),
      x(nullptr) { }

void FixedRadiusExtrapolationWidgetImplementation::adjustControls()
{
    // Labels
    lblBaseLevel->setText(FixedRadiusExtrapolationWidget::tr("Landscape base level:"));
    lblFixedRadius->setText(FixedRadiusExtrapolationWidget::tr("Extrapolation radius:"));

    lblBaseLevel->setBuddy(spnBaseLevel);
    lblFixedRadius->setBuddy(spnFixedRadius);

    // Spin-boxes
    spnBaseLevel->setDecimals(2);
    spnBaseLevel->setRange(static_cast<double>(Preferences::MinLevel),
                           static_cast<double>(Preferences::MaxLevel));
    spnBaseLevel->setSingleStep(0.01);
    spnBaseLevel->setAccelerated(true);
    spnBaseLevel->setAlignment(Qt::AlignRight);

    spnFixedRadius->setRange(1, Preferences::MaxLandscapeDim);
    spnFixedRadius->setSingleStep(1);
    spnFixedRadius->setAccelerated(true);
    spnFixedRadius->setAlignment(Qt::AlignRight);
}

void FixedRadiusExtrapolationWidgetImplementation::adjustLayout(FixedRadiusExtrapolationWidget *master)
{
    QGridLayout *lytContent = new QGridLayout;
    lytContent->setColumnStretch(0, 1);
    lytContent->addWidget(lblBaseLevel, 0, 1);
    lytContent->setColumnStretch(2, 1);
    lytContent->addWidget(spnBaseLevel, 0, 3);
    lytContent->setColumnStretch(4, 1);
    lytContent->setRowMinimumHeight(1, 12);
    lytContent->addWidget(lblFixedRadius, 2, 1);
    lytContent->addWidget(spnFixedRadius, 2, 3);

    QBoxLayout *lytMain = new QVBoxLayout(master);
    lytMain->addLayout(lytContent);
    lytMain->addStretch();

    FixedRadiusExtrapolationWidget::connect(
                spnBaseLevel, SIGNAL(valueChanged(double)), master, SLOT(setBaseLevel(double)));
    FixedRadiusExtrapolationWidget::connect(
                spnFixedRadius, SIGNAL(valueChanged(int)), master, SLOT(setFixedRadius(int)));
}

void FixedRadiusExtrapolationWidgetImplementation::adjustValues()
{
    spnBaseLevel->setValue(x->baseLevel());
    spnFixedRadius->setValue(x->fixedRadius());
}

FixedRadiusExtrapolationWidgetImplementation::~FixedRadiusExtrapolationWidgetImplementation() { }


FixedRadiusExtrapolationWidget::FixedRadiusExtrapolationWidget(QWidget *parent)
    : AbstractExtrapolationWidget(parent),
      m(new FixedRadiusExtrapolationWidgetImplementation)
{
    m->adjustControls();
    m->adjustLayout(this);
}


Extrapolator *FixedRadiusExtrapolationWidget::extrapolator() const
{ return m->x; }

void FixedRadiusExtrapolationWidget::bindExtrapolator(FixedRadiusExtrapolator *frx)
{
    m->x = frx;
    m->adjustValues();
}

bool FixedRadiusExtrapolationWidget::directInfluence() const
{ return m->infl; }

void FixedRadiusExtrapolationWidget::setDirectInfluence(bool infl)
{ m->infl = infl; }

void FixedRadiusExtrapolationWidget::acceptSettings()
{
    m->x->setBaseLevel(m->bl);
    m->x->setFixedRadius(m->fr);
}


FixedRadiusExtrapolationWidget::~FixedRadiusExtrapolationWidget()
{
    delete m;
}


void FixedRadiusExtrapolationWidget::setBaseLevel(double bl)
{
    m->bl = bl;
    if (m->infl)
        m->x->setBaseLevel(m->bl);
}

void FixedRadiusExtrapolationWidget::setFixedRadius(int fr)
{
    m->fr = fr;
    if (m->infl)
        m->x->setFixedRadius(m->fr);
}


} // namespace HeightMap
