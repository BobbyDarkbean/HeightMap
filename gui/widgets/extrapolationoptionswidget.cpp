#include <QLabel>
#include <QSpinBox>
#include <QBoxLayout>
#include <QGroupBox>
#include "../preferences.h"

#include "extrapolationoptionswidget.h"


namespace HeightMap {


struct ExtrapolationOptionsWidgetImplementation
{
    ExtrapolationOptionsWidgetImplementation();

    void adjustControls();
    void adjustLayout(QWidget *master);

    ~ExtrapolationOptionsWidgetImplementation();

    QGroupBox *grpExtrapolOpts;
    QLabel *lblBaseLevel;
    QSpinBox *spnBaseLevel;

private:
    DISABLE_COPY(ExtrapolationOptionsWidgetImplementation)
    DISABLE_MOVE(ExtrapolationOptionsWidgetImplementation)
};


ExtrapolationOptionsWidgetImplementation::ExtrapolationOptionsWidgetImplementation()
    : grpExtrapolOpts(new QGroupBox),
      lblBaseLevel(new QLabel),
      spnBaseLevel(new QSpinBox) { }

void ExtrapolationOptionsWidgetImplementation::adjustControls()
{
    // Label
    lblBaseLevel->setText(ExtrapolationOptionsWidget::tr("Landscape base level:"));
    lblBaseLevel->setBuddy(spnBaseLevel);

    // Spin-box
    spnBaseLevel->setRange(Preferences::MinLevel, Preferences::MaxLevel);
    spnBaseLevel->setSingleStep(1);
    spnBaseLevel->setAccelerated(true);
    spnBaseLevel->setAlignment(Qt::AlignRight);

    // Group-box
    grpExtrapolOpts->setTitle(ExtrapolationOptionsWidget::tr("Peak extrapolation"));
}

void ExtrapolationOptionsWidgetImplementation::adjustLayout(QWidget *master)
{
    QBoxLayout *lytBaseLevel = new QHBoxLayout(grpExtrapolOpts);
    lytBaseLevel->addStretch();
    lytBaseLevel->addWidget(lblBaseLevel);
    lytBaseLevel->addWidget(spnBaseLevel);
    lytBaseLevel->addStretch();

    QBoxLayout *lytMain = new QVBoxLayout(master);
    lytMain->addWidget(grpExtrapolOpts);
    lytMain->addStretch();
}

ExtrapolationOptionsWidgetImplementation::~ExtrapolationOptionsWidgetImplementation() { }


ExtrapolationOptionsWidget::ExtrapolationOptionsWidget(QWidget *parent)
    : QWidget(parent),
      m(new ExtrapolationOptionsWidgetImplementation)
{
    m->adjustControls();
    m->adjustLayout(this);
}


int ExtrapolationOptionsWidget::baseLevel() const
{ return m->spnBaseLevel->value(); }

void ExtrapolationOptionsWidget::setBaseLevel(int level)
{ m->spnBaseLevel->setValue(level); }


ExtrapolationOptionsWidget::~ExtrapolationOptionsWidget()
{
    delete m;
}


} // namespace HeightMap
