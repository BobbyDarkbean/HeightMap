#include <QLabel>
#include <QSpinBox>
#include <QGridLayout>
#include "extrapolator.h"
#include "extrapolationdata.h"
#include "preferences.h"

#include "slopeextrapolationwidget.h"


namespace HeightMap {


struct SlopeExtrapolationWidgetImplementation
{
    SlopeExtrapolationWidgetImplementation();

    void adjustControls(QWidget *master);
    void adjustLayout(QWidget *master);
    void adjustValues();

    ExtrapolationData extract() const;
    void provide(const ExtrapolationData &);

    ~SlopeExtrapolationWidgetImplementation();

    QLabel *lblBaseLevel;
    QDoubleSpinBox *spnBaseLevel;

    QLabel *lblSlope;
    QDoubleSpinBox *spnSlope;

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
      x(nullptr) { }

void SlopeExtrapolationWidgetImplementation::adjustControls(QWidget *master)
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
    spnBaseLevel->installEventFilter(master);

    spnSlope->setDecimals(2);
    spnSlope->setRange(1.0, 32.0);
    spnSlope->setSingleStep(0.01);
    spnSlope->setAccelerated(true);
    spnSlope->setAlignment(Qt::AlignRight);
    spnSlope->installEventFilter(master);
}

void SlopeExtrapolationWidgetImplementation::adjustLayout(QWidget *master)
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
}

void SlopeExtrapolationWidgetImplementation::adjustValues()
{
    spnBaseLevel->setValue(x->baseLevel());
    spnSlope->setValue(x->slopeRatio());
}

ExtrapolationData SlopeExtrapolationWidgetImplementation::extract() const
{
    ExtrapolationData data;
    data.insert("baselevel", spnBaseLevel->value());
    data.insert("ratio", spnSlope->value());
    return data;
}

void SlopeExtrapolationWidgetImplementation::provide(const ExtrapolationData &xdata)
{
    spnBaseLevel->setValue(xdata.value("baselevel", -1.0));
    spnSlope->setValue(xdata.value("ratio", -1.0));
}

SlopeExtrapolationWidgetImplementation::~SlopeExtrapolationWidgetImplementation() { }


SlopeExtrapolationWidget::SlopeExtrapolationWidget(QWidget *parent)
    : AbstractExtrapolationWidget(parent),
      m(new SlopeExtrapolationWidgetImplementation)
{
    m->adjustControls(this);
    m->adjustLayout(this);
}


Extrapolator *SlopeExtrapolationWidget::extrapolator() const
{ return m->x; }

void SlopeExtrapolationWidget::bindExtrapolator(SlopeExtrapolator *sx)
{
    m->x = sx;
    refreshData();

    typedef SlopeExtrapolationWidget W;
    void (QDoubleSpinBox::*qDoubleSpinBoxValueChanged)(double) = &QDoubleSpinBox::valueChanged;

    connect(m->spnBaseLevel,    qDoubleSpinBoxValueChanged, this,   &W::setBaseLevel);
    connect(m->spnSlope,        qDoubleSpinBoxValueChanged, this,   &W::setSlopeRatio);
}

void SlopeExtrapolationWidget::refreshData()
{ m->adjustValues(); }

ExtrapolationData SlopeExtrapolationWidget::extractData() const
{ return m->extract(); }

void SlopeExtrapolationWidget::provideData(const ExtrapolationData &xdata)
{ m->provide(xdata); }


SlopeExtrapolationWidget::~SlopeExtrapolationWidget()
{
    delete m;
}


void SlopeExtrapolationWidget::setBaseLevel(double bl)
{
    m->x->setBaseLevel(bl);
}

void SlopeExtrapolationWidget::setSlopeRatio(double sr)
{
    m->x->setSlopeRatio(sr);
}


} // namespace HeightMap
