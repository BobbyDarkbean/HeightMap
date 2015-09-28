#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QStackedWidget>
#include <QBoxLayout>
#include "../preferences.h"

#include "extrapolationoptionswidget.h"


namespace HeightMap {


struct ExtrapolationOptionsWidgetImplementation
{
    ExtrapolationOptionsWidgetImplementation();

    void adjustControls();
    void adjustLayout(ExtrapolationOptionsWidget *master);

    ~ExtrapolationOptionsWidgetImplementation();

    QLabel *lblExtrapolMethod;
    QComboBox *cmbExtrapolMethod;

    QGroupBox *grpExtrapolOpts;
    QStackedWidget *stkExtrapolOpts;

    QMap<QString, int> mapExtrapolNames;

private:
    DISABLE_COPY(ExtrapolationOptionsWidgetImplementation)
    DISABLE_MOVE(ExtrapolationOptionsWidgetImplementation)
};


ExtrapolationOptionsWidgetImplementation::ExtrapolationOptionsWidgetImplementation()
    : lblExtrapolMethod(new QLabel),
      cmbExtrapolMethod(new QComboBox),
      grpExtrapolOpts(new QGroupBox),
      stkExtrapolOpts(new QStackedWidget),
      mapExtrapolNames() { }

void ExtrapolationOptionsWidgetImplementation::adjustControls()
{
    // Label
    lblExtrapolMethod->setText(ExtrapolationOptionsWidget::tr("Extrapolation method:"));
    lblExtrapolMethod->setBuddy(cmbExtrapolMethod);

    // Combo-box
    cmbExtrapolMethod->setEditable(false);

    // Group-box
    grpExtrapolOpts->setTitle(ExtrapolationOptionsWidget::tr("Peak extrapolation"));
}

void ExtrapolationOptionsWidgetImplementation::adjustLayout(ExtrapolationOptionsWidget *master)
{
    QBoxLayout *lytExtrapolMethod = new QHBoxLayout;
    lytExtrapolMethod->addStretch();
    lytExtrapolMethod->addWidget(lblExtrapolMethod);
    lytExtrapolMethod->addWidget(cmbExtrapolMethod);
    lytExtrapolMethod->addStretch();

    QBoxLayout *lytExtrapolOpts = new QVBoxLayout(grpExtrapolOpts);
    lytExtrapolOpts->addWidget(stkExtrapolOpts);

    QBoxLayout *lytMain = new QVBoxLayout(master);
    lytMain->addWidget(grpExtrapolOpts);
    lytMain->addLayout(lytExtrapolMethod);
    lytMain->addStretch();

    ExtrapolationOptionsWidget::connect(
            cmbExtrapolMethod, SIGNAL(currentIndexChanged(int)), master, SLOT(setExtrapolationWidget(int)));
}

ExtrapolationOptionsWidgetImplementation::~ExtrapolationOptionsWidgetImplementation() { }


ExtrapolationOptionsWidget::ExtrapolationOptionsWidget(QWidget *parent)
    : QWidget(parent),
      m(new ExtrapolationOptionsWidgetImplementation)
{
    m->adjustControls();
    m->adjustLayout(this);
}


QString ExtrapolationOptionsWidget::extrapolatorName() const
{ return m->cmbExtrapolMethod->currentText(); }

void ExtrapolationOptionsWidget::setExtrapolatorName(const QString &name)
{
    int index = m->mapExtrapolNames.value(name, -1);
    if (0 <= index && index < m->cmbExtrapolMethod->count())
        m->cmbExtrapolMethod->setCurrentIndex(index);
}

QWidget *ExtrapolationOptionsWidget::extrapolationWidget(const QString &keyName) const
{
    int index = m->mapExtrapolNames.value(keyName, -1);
    int id = m->cmbExtrapolMethod->itemData(index).toInt();
    if (!(0 <= id && id < m->stkExtrapolOpts->count()))
        return nullptr;
    return m->stkExtrapolOpts->widget(id);
}

void ExtrapolationOptionsWidget::addExtrapolationWidget(
    const QString &keyName,
    QWidget *w)
{
    int id = m->stkExtrapolOpts->addWidget(w);
    m->mapExtrapolNames.insert(keyName, id);
    m->cmbExtrapolMethod->addItem(keyName, id);
}


ExtrapolationOptionsWidget::~ExtrapolationOptionsWidget()
{
    delete m;
}


void ExtrapolationOptionsWidget::setExtrapolationWidget(int index)
{
    int id = m->cmbExtrapolMethod->itemData(index).toInt();
    if (0 <= id && id < m->stkExtrapolOpts->count())
        m->stkExtrapolOpts->setCurrentIndex(id);
}


} // namespace HeightMap
