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
    void adjustLayout(BaseLevelExtrapolationWidget *master);
    void adjustValues();

    ~BaseLevelExtrapolationWidgetImplementation();

    QLabel *lblBaseLevel;
    QDoubleSpinBox *spnBaseLevel;

    double bl;
    bool infl;

    BaseLevelExtrapolator *x;

private:
    DISABLE_COPY(BaseLevelExtrapolationWidgetImplementation)
    DISABLE_MOVE(BaseLevelExtrapolationWidgetImplementation)
};


BaseLevelExtrapolationWidgetImplementation::BaseLevelExtrapolationWidgetImplementation()
    : lblBaseLevel(new QLabel),
      spnBaseLevel(new QDoubleSpinBox),
      bl(static_cast<double>(Preferences::MinLevel)),
      infl(false),
      x(nullptr) { }

void BaseLevelExtrapolationWidgetImplementation::adjustControls()
{
    // Label
    lblBaseLevel->setText(BaseLevelExtrapolationWidget::tr("Landscape base level:"));
    lblBaseLevel->setBuddy(spnBaseLevel);

    // Spin-box
    spnBaseLevel->setDecimals(2);
    spnBaseLevel->setRange(static_cast<double>(Preferences::MinLevel),
                           static_cast<double>(Preferences::MaxLevel));
    spnBaseLevel->setSingleStep(0.01);
    spnBaseLevel->setAccelerated(true);
    spnBaseLevel->setAlignment(Qt::AlignRight);
}

void BaseLevelExtrapolationWidgetImplementation::adjustLayout(BaseLevelExtrapolationWidget *master)
{
    QGridLayout *lytContent = new QGridLayout;
    lytContent->setColumnStretch(0, 1);
    lytContent->addWidget(lblBaseLevel, 0, 1);
    lytContent->setColumnStretch(2, 1);
    lytContent->addWidget(spnBaseLevel, 0, 3);
    lytContent->setColumnStretch(4, 1);

    QBoxLayout *lytMain = new QVBoxLayout(master);
    lytMain->addLayout(lytContent);
    lytMain->addStretch();

    BaseLevelExtrapolationWidget::connect(
                spnBaseLevel, SIGNAL(valueChanged(double)), master, SLOT(setBaseLevel(double)));
}

void BaseLevelExtrapolationWidgetImplementation::adjustValues()
{
    spnBaseLevel->setValue(x->baseLevel());
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
{
    m->x = blx;
    m->adjustValues();
}

bool BaseLevelExtrapolationWidget::directInfluence() const
{ return m->infl; }

void BaseLevelExtrapolationWidget::setDirectInfluence(bool infl)
{ m->infl = infl; }

void BaseLevelExtrapolationWidget::acceptSettings()
{ m->x->setBaseLevel(m->bl); }


BaseLevelExtrapolationWidget::~BaseLevelExtrapolationWidget()
{
    delete m;
}


void BaseLevelExtrapolationWidget::setBaseLevel(double bl)
{
    m->bl = bl;
    if (m->infl)
        m->x->setBaseLevel(m->bl);
}


} // namespace HeightMap
