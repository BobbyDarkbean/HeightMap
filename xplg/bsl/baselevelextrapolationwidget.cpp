#include <QLabel>
#include <QSpinBox>
#include <QGridLayout>
#include "extrapolator.h"
#include "extrapolationdata.h"
#include "preferences.h"

#include "baselevelextrapolationwidget.h"


namespace HeightMap {


struct BaseLevelExtrapolationWidgetImplementation
{
    BaseLevelExtrapolationWidgetImplementation();

    void adjustControls();
    void adjustLayout(QWidget *master);
    void adjustValues();

    ExtrapolationData extract() const;
    void provide(const ExtrapolationData &);

    ~BaseLevelExtrapolationWidgetImplementation();

    QLabel *lblBaseLevel;
    QDoubleSpinBox *spnBaseLevel;

    BaseLevelExtrapolator *x;

private:
    DISABLE_COPY(BaseLevelExtrapolationWidgetImplementation)
    DISABLE_MOVE(BaseLevelExtrapolationWidgetImplementation)
};


BaseLevelExtrapolationWidgetImplementation::BaseLevelExtrapolationWidgetImplementation()
    : lblBaseLevel(new QLabel),
      spnBaseLevel(new QDoubleSpinBox),
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

void BaseLevelExtrapolationWidgetImplementation::adjustLayout(QWidget *master)
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

void BaseLevelExtrapolationWidgetImplementation::adjustValues()
{
    spnBaseLevel->setValue(x->baseLevel());
}

ExtrapolationData BaseLevelExtrapolationWidgetImplementation::extract() const
{
    ExtrapolationData data;
    data.insert("baselevel", spnBaseLevel->value());
    return data;
}

void BaseLevelExtrapolationWidgetImplementation::provide(const ExtrapolationData &xdata)
{
    spnBaseLevel->setValue(xdata.value("baselevel", -1.0));
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
    refreshData();

    typedef BaseLevelExtrapolationWidget W;
    void (QDoubleSpinBox::*qDoubleSpinBoxValueChanged)(double) = &QDoubleSpinBox::valueChanged;

    connect(m->spnBaseLevel,    qDoubleSpinBoxValueChanged, this,   &W::setBaseLevel);
}

void BaseLevelExtrapolationWidget::refreshData()
{ m->adjustValues(); }

ExtrapolationData BaseLevelExtrapolationWidget::extractData() const
{ return m->extract(); }

void BaseLevelExtrapolationWidget::provideData(const ExtrapolationData &xdata)
{ m->provide(xdata); }


BaseLevelExtrapolationWidget::~BaseLevelExtrapolationWidget()
{
    delete m;
}


void BaseLevelExtrapolationWidget::setBaseLevel(double bl)
{
    m->x->setBaseLevel(bl);
}


} // namespace HeightMap
