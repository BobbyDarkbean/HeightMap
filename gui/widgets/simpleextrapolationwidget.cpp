#include <QLabel>
#include <QSpinBox>
#include <QGridLayout>
#include "extrapolator.h"
#include "../extrapolation/extrapolationdata.h"
#include "../preferences.h"

#include "simpleextrapolationwidget.h"


namespace HeightMap {


struct SimpleExtrapolationWidgetImplementation
{
    SimpleExtrapolationWidgetImplementation();

    void adjustControls();
    void adjustLayout(QWidget *master);
    void adjustValues();

    ExtrapolationData extract() const;
    void provide(const ExtrapolationData &);

    ~SimpleExtrapolationWidgetImplementation();

    QLabel *lblBaseLevel;
    QDoubleSpinBox *spnBaseLevel;

    SimpleExtrapolator *x;

private:
    DISABLE_COPY(SimpleExtrapolationWidgetImplementation)
    DISABLE_MOVE(SimpleExtrapolationWidgetImplementation)
};


SimpleExtrapolationWidgetImplementation::SimpleExtrapolationWidgetImplementation()
    : lblBaseLevel(new QLabel),
      spnBaseLevel(new QDoubleSpinBox),
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

void SimpleExtrapolationWidgetImplementation::adjustLayout(QWidget *master)
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
}

void SimpleExtrapolationWidgetImplementation::adjustValues()
{
    spnBaseLevel->setValue(x->baseLevel());
}

ExtrapolationData SimpleExtrapolationWidgetImplementation::extract() const
{
    ExtrapolationData data;
    data.insert("baselevel", spnBaseLevel->value());
    return data;
}

void SimpleExtrapolationWidgetImplementation::provide(const ExtrapolationData &xdata)
{
    spnBaseLevel->setValue(xdata.value("baselevel", -1.0));
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

    typedef SimpleExtrapolationWidget W;
    void (QDoubleSpinBox::*qDoubleSpinBoxValueChanged)(double) = &QDoubleSpinBox::valueChanged;

    connect(m->spnBaseLevel,    qDoubleSpinBoxValueChanged, this,   &W::setBaseLevel);
}

void SimpleExtrapolationWidget::refreshData()
{ m->adjustValues(); }

ExtrapolationData SimpleExtrapolationWidget::extractData() const
{ return m->extract(); }

void SimpleExtrapolationWidget::provideData(const ExtrapolationData &xdata)
{ m->provide(xdata); }


SimpleExtrapolationWidget::~SimpleExtrapolationWidget()
{
    delete m;
}


void SimpleExtrapolationWidget::setBaseLevel(double bl)
{
    m->x->setBaseLevel(bl);
}


} // namespace HeightMap
