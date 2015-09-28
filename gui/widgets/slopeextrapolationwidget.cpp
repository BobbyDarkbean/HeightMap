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
    void adjustLayout(QWidget *master);

    ~SlopeExtrapolationWidgetImplementation();

    QLabel *lblBaseLevel;
    QSpinBox *spnBaseLevel;

    QLabel *lblSlope;
    QSpinBox *spnSlope;

    SlopeExtrapolator *x;

private:
    DISABLE_COPY(SlopeExtrapolationWidgetImplementation)
    DISABLE_MOVE(SlopeExtrapolationWidgetImplementation)
};


SlopeExtrapolationWidgetImplementation::SlopeExtrapolationWidgetImplementation()
    : lblBaseLevel(new QLabel),
      spnBaseLevel(new QSpinBox),
      lblSlope(new QLabel),
      spnSlope(new QSpinBox),
      x(nullptr) { }

void SlopeExtrapolationWidgetImplementation::adjustControls()
{
    // Labels
    lblBaseLevel->setText(SlopeExtrapolationWidget::tr("Landscape base level:"));
    lblSlope->setText(SlopeExtrapolationWidget::tr("Slope ratio:"));

    lblBaseLevel->setBuddy(spnBaseLevel);
    lblSlope->setBuddy(spnSlope);

    // Spin-boxes
    spnBaseLevel->setRange(Preferences::MinLevel, Preferences::MaxLevel);
    spnBaseLevel->setSingleStep(1);
    spnBaseLevel->setAccelerated(true);
    spnBaseLevel->setAlignment(Qt::AlignRight);

    spnSlope->setRange(1, 32);
    spnSlope->setSingleStep(1);
    spnSlope->setAccelerated(true);
    spnSlope->setAlignment(Qt::AlignRight);
}

void SlopeExtrapolationWidgetImplementation::adjustLayout(QWidget *master)
{
    QGridLayout *lytMain = new QGridLayout(master);
    lytMain->setColumnStretch(0, 0);
    lytMain->addWidget(lblBaseLevel, 0, 1);
    lytMain->addWidget(spnBaseLevel, 0, 2);
    lytMain->setColumnStretch(3, 0);
    lytMain->setRowMinimumHeight(1, 12);
    lytMain->addWidget(lblSlope, 2, 1);
    lytMain->addWidget(spnSlope, 2, 2);
    lytMain->setRowStretch(3, 0);
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
{ m->x = sx; }


SlopeExtrapolationWidget::~SlopeExtrapolationWidget()
{
    delete m;
}


} // namespace HeightMap
