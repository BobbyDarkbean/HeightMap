#include <QLabel>
#include <QSpinBox>
#include <QGridLayout>
#include "extrapolator.h"
#include "extrapolationdata.h"
#include "preferences.h"

#include "fixedradiusextrapolationwidget.h"


namespace HeightMap {


struct FixedRadiusExtrapolationWidgetImplementation
{
    FixedRadiusExtrapolationWidgetImplementation();

    void adjustControls(QWidget *master);
    void adjustLayout(QWidget *master);
    void adjustValues();

    ExtrapolationData extract() const;
    void provide(const ExtrapolationData &);

    ~FixedRadiusExtrapolationWidgetImplementation();

    QLabel *lblBaseLevel;
    QDoubleSpinBox *spnBaseLevel;

    QLabel *lblFixedRadius;
    QSpinBox *spnFixedRadius;

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
      x(nullptr) { }

void FixedRadiusExtrapolationWidgetImplementation::adjustControls(QWidget *master)
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
    spnBaseLevel->installEventFilter(master);

    spnFixedRadius->setRange(1, Preferences::MaxLandscapeDim);
    spnFixedRadius->setSingleStep(1);
    spnFixedRadius->setAccelerated(true);
    spnFixedRadius->setAlignment(Qt::AlignRight);
    spnFixedRadius->installEventFilter(master);
}

void FixedRadiusExtrapolationWidgetImplementation::adjustLayout(QWidget *master)
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
}

void FixedRadiusExtrapolationWidgetImplementation::adjustValues()
{
    spnBaseLevel->setValue(x->baseLevel());
    spnFixedRadius->setValue(x->fixedRadius());
}

ExtrapolationData FixedRadiusExtrapolationWidgetImplementation::extract() const
{
    ExtrapolationData data;
    data.insert("baselevel", spnBaseLevel->value());
    data.insert("radius", spnFixedRadius->value());
    return data;
}

void FixedRadiusExtrapolationWidgetImplementation::provide(const ExtrapolationData &xdata)
{
    spnBaseLevel->setValue(xdata.value("baselevel", -1.0));
    spnFixedRadius->setValue(static_cast<int>(xdata.value("radius", -1.0)));
}

FixedRadiusExtrapolationWidgetImplementation::~FixedRadiusExtrapolationWidgetImplementation() { }


FixedRadiusExtrapolationWidget::FixedRadiusExtrapolationWidget(QWidget *parent)
    : AbstractExtrapolationWidget(parent),
      m(new FixedRadiusExtrapolationWidgetImplementation)
{
    m->adjustControls(this);
    m->adjustLayout(this);
}


Extrapolator *FixedRadiusExtrapolationWidget::extrapolator() const
{ return m->x; }

void FixedRadiusExtrapolationWidget::bindExtrapolator(FixedRadiusExtrapolator *frx)
{
    m->x = frx;
    refreshData();

    typedef FixedRadiusExtrapolationWidget W;
    void (QDoubleSpinBox::*qDoubleSpinBoxValueChanged)(double) = &QDoubleSpinBox::valueChanged;
    void (QSpinBox::*qSpinBoxValueChanged)(int) = &QSpinBox::valueChanged;

    connect(m->spnBaseLevel,    qDoubleSpinBoxValueChanged, this,   &W::setBaseLevel);
    connect(m->spnFixedRadius,  qSpinBoxValueChanged,       this,   &W::setFixedRadius);
}

void FixedRadiusExtrapolationWidget::refreshData()
{ m->adjustValues(); }

ExtrapolationData FixedRadiusExtrapolationWidget::extractData() const
{ return m->extract(); }

void FixedRadiusExtrapolationWidget::provideData(const ExtrapolationData &xdata)
{ m->provide(xdata); }


FixedRadiusExtrapolationWidget::~FixedRadiusExtrapolationWidget()
{
    delete m;
}


void FixedRadiusExtrapolationWidget::setBaseLevel(double bl)
{
    m->x->setBaseLevel(bl);
}

void FixedRadiusExtrapolationWidget::setFixedRadius(int fr)
{
    m->x->setFixedRadius(fr);
}


} // namespace HeightMap
