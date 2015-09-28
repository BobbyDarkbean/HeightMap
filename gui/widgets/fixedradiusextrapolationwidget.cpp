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
    void adjustLayout(QWidget *master);

    ~FixedRadiusExtrapolationWidgetImplementation();

    QLabel *lblBaseLevel;
    QSpinBox *spnBaseLevel;

    QLabel *lblFixedRadius;
    QSpinBox *spnFixedRadius;

    FixedRadiusExtrapolator *x;

private:
    DISABLE_COPY(FixedRadiusExtrapolationWidgetImplementation)
    DISABLE_MOVE(FixedRadiusExtrapolationWidgetImplementation)
};


FixedRadiusExtrapolationWidgetImplementation::FixedRadiusExtrapolationWidgetImplementation()
    : lblBaseLevel(new QLabel),
      spnBaseLevel(new QSpinBox),
      lblFixedRadius(new QLabel),
      spnFixedRadius(new QSpinBox),
      x(nullptr) { }

void FixedRadiusExtrapolationWidgetImplementation::adjustControls()
{
    // Labels
    lblBaseLevel->setText(FixedRadiusExtrapolationWidget::tr("Landscape base level:"));
    lblFixedRadius->setText(FixedRadiusExtrapolationWidget::tr("Extrapolation radius:"));

    lblBaseLevel->setBuddy(spnBaseLevel);
    lblFixedRadius->setBuddy(spnFixedRadius);

    // Spin-boxes
    spnBaseLevel->setRange(Preferences::MinLevel, Preferences::MaxLevel);
    spnBaseLevel->setSingleStep(1);
    spnBaseLevel->setAccelerated(true);
    spnBaseLevel->setAlignment(Qt::AlignRight);

    spnFixedRadius->setRange(1, Preferences::MaxLandscapeDim);
    spnFixedRadius->setSingleStep(1);
    spnFixedRadius->setAccelerated(true);
    spnFixedRadius->setAlignment(Qt::AlignRight);
}

void FixedRadiusExtrapolationWidgetImplementation::adjustLayout(QWidget *master)
{
    QGridLayout *lytMain = new QGridLayout(master);
    lytMain->setColumnStretch(0, 0);
    lytMain->addWidget(lblBaseLevel, 0, 1);
    lytMain->addWidget(spnBaseLevel, 0, 2);
    lytMain->setColumnStretch(3, 0);
    lytMain->setRowMinimumHeight(1, 12);
    lytMain->addWidget(lblFixedRadius, 2, 1);
    lytMain->addWidget(spnFixedRadius, 2, 2);
    lytMain->setRowStretch(3, 0);
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
{ m->x = frx; }


FixedRadiusExtrapolationWidget::~FixedRadiusExtrapolationWidget()
{
    delete m;
}


} // namespace HeightMap
