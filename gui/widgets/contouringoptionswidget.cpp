#include <QLabel>
#include <QSpinBox>
#include <QGroupBox>
#include <QLayout>
#include "../preferences.h"

#include "contouringoptionswidget.h"


namespace HeightMap {


struct ContouringOptionsWidgetImplementation
{
    ContouringOptionsWidgetImplementation();

    void adjustControls();
    void adjustLayout(QWidget *master);

    ~ContouringOptionsWidgetImplementation();

    QGroupBox *grpLevelOpts;
    QLabel *lblMinLevel;
    QSpinBox *spnMinLevel;
    QLabel *lblMaxLevel;
    QSpinBox *spnMaxLevel;
    QLabel *lblStep;
    QSpinBox *spnStep;

private:
    DISABLE_COPY(ContouringOptionsWidgetImplementation)
    DISABLE_MOVE(ContouringOptionsWidgetImplementation)
};


ContouringOptionsWidgetImplementation::ContouringOptionsWidgetImplementation()
    : grpLevelOpts(new QGroupBox),
      lblMinLevel(new QLabel),
      spnMinLevel(new QSpinBox),
      lblMaxLevel(new QLabel),
      spnMaxLevel(new QSpinBox),
      lblStep(new QLabel),
      spnStep(new QSpinBox) { }

void ContouringOptionsWidgetImplementation::adjustControls()
{
    // Labels
    lblMinLevel->setText(ContouringOptionsWidget::tr("Min. level"));
    lblMaxLevel->setText(ContouringOptionsWidget::tr("Max. level"));
    lblStep->setText(ContouringOptionsWidget::tr("Step"));

    lblMinLevel->setBuddy(spnMinLevel);
    lblMaxLevel->setBuddy(spnMaxLevel);
    lblStep->setBuddy(spnStep);

    // Spin-boxes
    spnMinLevel->setRange(1, Preferences::MaxLevel);
    spnMaxLevel->setRange(1, Preferences::MaxLevel);
    spnStep->setRange(1, Preferences::MaxLevel);

    spnMinLevel->setSingleStep(1);
    spnMaxLevel->setSingleStep(1);
    spnStep->setSingleStep(1);

    spnMinLevel->setAccelerated(true);
    spnMaxLevel->setAccelerated(true);
    spnStep->setAccelerated(true);

    spnMinLevel->setAlignment(Qt::AlignRight);
    spnMaxLevel->setAlignment(Qt::AlignRight);
    spnStep->setAlignment(Qt::AlignRight);

    // Group-box
    grpLevelOpts->setTitle(ContouringOptionsWidget::tr("Levels"));
}

void ContouringOptionsWidgetImplementation::adjustLayout(QWidget *master)
{
    QGridLayout *lytLevels = new QGridLayout(grpLevelOpts);
    lytLevels->setColumnStretch(0, 1);
    lytLevels->addWidget(lblMinLevel, 0, 1);
    lytLevels->addWidget(spnMinLevel, 0, 2);
    lytLevels->setColumnMinimumWidth(3, 12);
    lytLevels->setColumnStretch(3, 1);
    lytLevels->addWidget(lblMaxLevel, 0, 4);
    lytLevels->addWidget(spnMaxLevel, 0, 5);
    lytLevels->setColumnStretch(6, 1);
    lytLevels->addWidget(lblStep, 2, 4);
    lytLevels->addWidget(spnStep, 2, 5);

    QBoxLayout *lytMain = new QVBoxLayout(master);
    lytMain->addWidget(grpLevelOpts);
    lytMain->addStretch();
}

ContouringOptionsWidgetImplementation::~ContouringOptionsWidgetImplementation() { }


ContouringOptionsWidget::ContouringOptionsWidget(QWidget *parent)
    : QWidget(parent),
      m(new ContouringOptionsWidgetImplementation)
{
    m->adjustControls();
    m->adjustLayout(this);
}


int ContouringOptionsWidget::minLevel() const
{ return m->spnMinLevel->value(); }

void ContouringOptionsWidget::setMinLevel(int level)
{ m->spnMinLevel->setValue(level); }

int ContouringOptionsWidget::maxLevel() const
{ return m->spnMaxLevel->value(); }

void ContouringOptionsWidget::setMaxLevel(int level)
{ m->spnMaxLevel->setValue(level); }

int ContouringOptionsWidget::step() const
{ return m->spnStep->value(); }

void ContouringOptionsWidget::setStep(int value)
{ m->spnStep->setValue(value); }


ContouringOptionsWidget::~ContouringOptionsWidget()
{
    delete m;
}


} // namespace HeightMap