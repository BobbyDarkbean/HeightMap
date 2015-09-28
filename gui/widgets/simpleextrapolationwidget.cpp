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
    void adjustLayout(QWidget *master);

    ~SimpleExtrapolationWidgetImplementation();

    QLabel *lblBaseLevel;
    QSpinBox *spnBaseLevel;

    SimpleExtrapolator *x;

private:
    DISABLE_COPY(SimpleExtrapolationWidgetImplementation)
    DISABLE_MOVE(SimpleExtrapolationWidgetImplementation)
};


SimpleExtrapolationWidgetImplementation::SimpleExtrapolationWidgetImplementation()
    : lblBaseLevel(new QLabel),
      spnBaseLevel(new QSpinBox),
      x(nullptr) { }

void SimpleExtrapolationWidgetImplementation::adjustControls()
{
    // Label
    lblBaseLevel->setText(SimpleExtrapolationWidget::tr("Landscape base level:"));
    lblBaseLevel->setBuddy(spnBaseLevel);

    // Spin-box
    spnBaseLevel->setRange(Preferences::MinLevel, Preferences::MaxLevel);
    spnBaseLevel->setSingleStep(1);
    spnBaseLevel->setAccelerated(true);
    spnBaseLevel->setAlignment(Qt::AlignRight);
}

void SimpleExtrapolationWidgetImplementation::adjustLayout(QWidget *master)
{
    QGridLayout *lytMain = new QGridLayout(master);
    lytMain->addWidget(lblBaseLevel, 0, 1);
    lytMain->addWidget(spnBaseLevel, 0, 2);
    lytMain->setRowMinimumHeight(1, 12);
    lytMain->setColumnStretch(0, 0);
    lytMain->setColumnStretch(3, 0);
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
{ m->x = sx; }


SimpleExtrapolationWidget::~SimpleExtrapolationWidget()
{
    delete m;
}


} // namespace HeightMap
