#include <QLabel>
#include <QSpinBox>
#include <QGroupBox>
#include <QBoxLayout>
#include "../preferences.h"

#include "landscapeoptionswidget.h"


namespace HeightMap {


struct LandscapeOptionsWidgetImplementation
{
    LandscapeOptionsWidgetImplementation();

    void adjustControls();
    void adjustLayout(QWidget *master);

    ~LandscapeOptionsWidgetImplementation();

    QGroupBox *grpLandscape;
    QLabel *lblLandscapeWidth;
    QSpinBox *spnLandscapeWidth;
    QLabel *lblLandscapeHeight;
    QSpinBox *spnLandscapeHeight;

private:
    DISABLE_COPY(LandscapeOptionsWidgetImplementation)
    DISABLE_MOVE(LandscapeOptionsWidgetImplementation)
};


LandscapeOptionsWidgetImplementation::LandscapeOptionsWidgetImplementation()
    : grpLandscape(new QGroupBox),
      lblLandscapeWidth(new QLabel),
      spnLandscapeWidth(new QSpinBox),
      lblLandscapeHeight(new QLabel),
      spnLandscapeHeight(new QSpinBox) { }

void LandscapeOptionsWidgetImplementation::adjustControls()
{
    // Labels
    lblLandscapeWidth->setText(LandscapeOptionsWidget::tr("Width:"));
    lblLandscapeHeight->setText(LandscapeOptionsWidget::tr("Height:"));

    lblLandscapeWidth->setBuddy(spnLandscapeWidth);
    lblLandscapeHeight->setBuddy(spnLandscapeHeight);

    // Spin-boxes
    spnLandscapeWidth->setRange(Preferences::MinLandscapeDim, Preferences::MaxLandscapeDim);
    spnLandscapeHeight->setRange(Preferences::MinLandscapeDim, Preferences::MaxLandscapeDim);

    spnLandscapeWidth->setSingleStep(1);
    spnLandscapeHeight->setSingleStep(1);

    spnLandscapeWidth->setAccelerated(true);
    spnLandscapeHeight->setAccelerated(true);

    spnLandscapeWidth->setAlignment(Qt::AlignRight);
    spnLandscapeHeight->setAlignment(Qt::AlignRight);

    // Group-box
    grpLandscape->setTitle(QObject::tr("Landscape"));
}

void LandscapeOptionsWidgetImplementation::adjustLayout(QWidget *master)
{
    QBoxLayout *lytLandscape = new QHBoxLayout(grpLandscape);
    lytLandscape->addStretch();
    lytLandscape->addWidget(lblLandscapeWidth);
    lytLandscape->addWidget(spnLandscapeWidth);
    lytLandscape->addStretch();
    lytLandscape->addWidget(lblLandscapeHeight);
    lytLandscape->addWidget(spnLandscapeHeight);
    lytLandscape->addStretch();

    QBoxLayout *lytMain = new QVBoxLayout(master);
    lytMain->addWidget(grpLandscape);
    lytMain->addStretch();
}

LandscapeOptionsWidgetImplementation::~LandscapeOptionsWidgetImplementation() { }


LandscapeOptionsWidget::LandscapeOptionsWidget(QWidget *parent)
    : QWidget(parent),
      m(new LandscapeOptionsWidgetImplementation)
{
    m->adjustControls();
    m->adjustLayout(this);
}


int LandscapeOptionsWidget::landscapeWidth() const
{ return m->spnLandscapeWidth->value(); }

void LandscapeOptionsWidget::setLandscapeWidth(int w)
{ m->spnLandscapeWidth->setValue(w); }

int LandscapeOptionsWidget::landscapeHeight() const
{ return m->spnLandscapeHeight->value(); }

void LandscapeOptionsWidget::setLandscapeHeight(int h)
{ m->spnLandscapeHeight->setValue(h); }


LandscapeOptionsWidget::~LandscapeOptionsWidget()
{
    delete m;
}


} // namespace HeightMap
