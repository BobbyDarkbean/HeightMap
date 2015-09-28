#include <QLabel>
#include <QSpinBox>
#include <QGridLayout>
#include "extrapolator.h"
#include "../preferences.h"

#include "baselevelextrapolationwidget.h"


namespace HeightMap {


struct BaseLevelExtrapolationWidgetImplementation
{
    BaseLevelExtrapolationWidgetImplementation();

    void adjustControls();
    void adjustLayout(QWidget *master);

    ~BaseLevelExtrapolationWidgetImplementation();

    QLabel *lblBaseLevel;
    QSpinBox *spnBaseLevel;

    BaseLevelExtrapolator *x;

private:
    DISABLE_COPY(BaseLevelExtrapolationWidgetImplementation)
    DISABLE_MOVE(BaseLevelExtrapolationWidgetImplementation)
};


BaseLevelExtrapolationWidgetImplementation::BaseLevelExtrapolationWidgetImplementation()
    : lblBaseLevel(new QLabel),
      spnBaseLevel(new QSpinBox),
      x(nullptr) { }

void BaseLevelExtrapolationWidgetImplementation::adjustControls()
{
    // Label
    lblBaseLevel->setText(BaseLevelExtrapolationWidget::tr("Landscape base level:"));
    lblBaseLevel->setBuddy(spnBaseLevel);

    // Spin-box
    spnBaseLevel->setRange(Preferences::MinLevel, Preferences::MaxLevel);
    spnBaseLevel->setSingleStep(1);
    spnBaseLevel->setAccelerated(true);
    spnBaseLevel->setAlignment(Qt::AlignRight);
}

void BaseLevelExtrapolationWidgetImplementation::adjustLayout(QWidget *master)
{
    QGridLayout *lytMain = new QGridLayout(master);
    lytMain->addWidget(lblBaseLevel, 0, 1);
    lytMain->addWidget(spnBaseLevel, 0, 2);
    lytMain->setRowMinimumHeight(1, 12);
    lytMain->setColumnStretch(0, 0);
    lytMain->setColumnStretch(3, 0);
}

BaseLevelExtrapolationWidgetImplementation::~BaseLevelExtrapolationWidgetImplementation() { }


BaseLevelExtrapolationWidget::BaseLevelExtrapolationWidget(QWidget *parent)
    : AbstractExtrapolationWidget(parent),
      m(new BaseLevelExtrapolationWidgetImplementation)
{
    m->adjustControls();
    m->adjustLayout(this);
}


Extrapolator *BaseLevelExtrapolationWidget::extrapolator() const
{ return m->x; }

void BaseLevelExtrapolationWidget::bindExtrapolator(BaseLevelExtrapolator *blx)
{ m->x = blx; }


BaseLevelExtrapolationWidget::~BaseLevelExtrapolationWidget()
{
    delete m;
}


} // namespace HeightMap
