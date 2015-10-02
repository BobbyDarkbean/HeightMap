#include <QLabel>
#include <QSpinBox>
#include <QGridLayout>
#include "extrapolator.h"
#include "../preferences.h"

#include "simpleextrapolationwidget.h"


namespace HeightMap {


struct SimpleExtrapolationWidgetImplementation
{
    SimpleExtrapolationWidgetImplementation();

    void adjustControls();
    void adjustLayout(SimpleExtrapolationWidget *master);
    void adjustValues();

    ~SimpleExtrapolationWidgetImplementation();

    QLabel *lblBaseLevel;
    QDoubleSpinBox *spnBaseLevel;

    double bl;

    SimpleExtrapolator *x;

private:
    DISABLE_COPY(SimpleExtrapolationWidgetImplementation)
    DISABLE_MOVE(SimpleExtrapolationWidgetImplementation)
};


SimpleExtrapolationWidgetImplementation::SimpleExtrapolationWidgetImplementation()
    : lblBaseLevel(new QLabel),
      spnBaseLevel(new QDoubleSpinBox),
      bl(static_cast<double>(Preferences::MinLevel)),
      x(nullptr) { }

void SimpleExtrapolationWidgetImplementation::adjustControls()
{
    // Label
    lblBaseLevel->setText(SimpleExtrapolationWidget::tr("Landscape base level:"));
    lblBaseLevel->setBuddy(spnBaseLevel);

    // Spin-box
    spnBaseLevel->setDecimals(2);
    spnBaseLevel->setRange(static_cast<double>(Preferences::MinLevel),
                           static_cast<double>(Preferences::MaxLevel));
    spnBaseLevel->setSingleStep(0.01);
    spnBaseLevel->setAccelerated(true);
    spnBaseLevel->setAlignment(Qt::AlignRight);
}

void SimpleExtrapolationWidgetImplementation::adjustLayout(SimpleExtrapolationWidget *master)
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

    SimpleExtrapolationWidget::connect(
                spnBaseLevel, SIGNAL(valueChanged(double)), master, SLOT(setBaseLevel(double)));
}

void SimpleExtrapolationWidgetImplementation::adjustValues()
{
    spnBaseLevel->setValue(x->baseLevel());
}

SimpleExtrapolationWidgetImplementation::~SimpleExtrapolationWidgetImplementation() { }


SimpleExtrapolationWidget::SimpleExtrapolationWidget(QWidget *parent)
    : AbstractExtrapolationWidget(parent),
      m(new SimpleExtrapolationWidgetImplementation)
{
    m->adjustControls();
    m->adjustLayout(this);
}


Extrapolator *SimpleExtrapolationWidget::extrapolator() const
{ return m->x; }

void SimpleExtrapolationWidget::bindExtrapolator(SimpleExtrapolator *sx)
{
    m->x = sx;
    refreshData();
}

void SimpleExtrapolationWidget::refreshData()
{ m->adjustValues(); }

void SimpleExtrapolationWidget::acceptSettings()
{ m->x->setBaseLevel(m->bl); }


SimpleExtrapolationWidget::~SimpleExtrapolationWidget()
{
    delete m;
}


void SimpleExtrapolationWidget::setBaseLevel(double bl)
{
    m->bl = bl;
}


} // namespace HeightMap
