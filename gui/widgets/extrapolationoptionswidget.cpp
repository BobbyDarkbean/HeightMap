#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QStackedWidget>
#include <QBoxLayout>
#include "abstractextrapolationwidget.h"
#include "../extrapolation/extrapolationfactory.h"
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

    QMap<int, AbstractExtrapolationWidget *> mapExtrapolWgts;
    QMap<QString, int> mapExtrapolId;

private:
    DISABLE_COPY(ExtrapolationOptionsWidgetImplementation)
    DISABLE_MOVE(ExtrapolationOptionsWidgetImplementation)
};


ExtrapolationOptionsWidgetImplementation::ExtrapolationOptionsWidgetImplementation()
    : lblExtrapolMethod(new QLabel),
      cmbExtrapolMethod(new QComboBox),
      grpExtrapolOpts(new QGroupBox),
      stkExtrapolOpts(new QStackedWidget),
      mapExtrapolWgts(),
      mapExtrapolId() { }

void ExtrapolationOptionsWidgetImplementation::adjustControls()
{
    // Label
    lblExtrapolMethod->setText(ExtrapolationOptionsWidget::tr("Extrapolation method:"));
    lblExtrapolMethod->setBuddy(cmbExtrapolMethod);

    // Combo-box
    cmbExtrapolMethod->setEditable(false);

    // Stacked widget
    stkExtrapolOpts->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    // Group-box
    grpExtrapolOpts->setTitle(ExtrapolationOptionsWidget::tr("Peak extrapolation"));
}

void ExtrapolationOptionsWidgetImplementation::adjustLayout(ExtrapolationOptionsWidget *master)
{
    QFrame *frmExtrapolMethod = new QFrame;

    QBoxLayout *lytExtrapolMethod = new QHBoxLayout(frmExtrapolMethod);
    lytExtrapolMethod->addStretch();
    lytExtrapolMethod->addWidget(lblExtrapolMethod);
    lytExtrapolMethod->addStretch();
    lytExtrapolMethod->addWidget(cmbExtrapolMethod);
    lytExtrapolMethod->addStretch();

    QBoxLayout *lytExtrapolOpts = new QVBoxLayout(grpExtrapolOpts);
    lytExtrapolOpts->addWidget(frmExtrapolMethod);
    lytExtrapolOpts->addWidget(stkExtrapolOpts);

    QBoxLayout *lytMain = new QVBoxLayout(master);
    lytMain->addWidget(grpExtrapolOpts);
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
{ return m->cmbExtrapolMethod->currentData().toString(); }

void ExtrapolationOptionsWidget::setExtrapolatorName(const QString &name)
{
    int index = m->cmbExtrapolMethod->findData(name);
    if (index != -1)
        m->cmbExtrapolMethod->setCurrentIndex(index);
}

QWidget *ExtrapolationOptionsWidget::extrapolationWidget(const QString &name) const
{
    int id = m->mapExtrapolId.value(name, -1);
    if (!(0 <= id && id < m->stkExtrapolOpts->count()))
        return nullptr;
    return m->stkExtrapolOpts->widget(id);
}

void ExtrapolationOptionsWidget::addExtrapolationWidget(ExtrapolationFactory *f, bool proxy)
{
    AbstractExtrapolationWidget *w = proxy ? f->createProxyWidget() : f->createWidget();
    QString name = f->name();
    QString description = f->description();

    int id = m->stkExtrapolOpts->addWidget(w);
    m->mapExtrapolWgts.insert(id, w);
    m->mapExtrapolId.insert(name, id);
    m->cmbExtrapolMethod->addItem(description, name);
}

void ExtrapolationOptionsWidget::retrieveExtrapolationSettings()
{
    for (auto i = m->mapExtrapolWgts.begin(); i != m->mapExtrapolWgts.end(); ++i) {
        i.value()->refreshData();
    }
}


ExtrapolationOptionsWidget::~ExtrapolationOptionsWidget()
{
    delete m;
}


void ExtrapolationOptionsWidget::setExtrapolationWidget(int index)
{
    QString name = m->cmbExtrapolMethod->itemData(index).toString();
    int id = m->mapExtrapolId.value(name, -1);
    if (0 <= id && id < m->stkExtrapolOpts->count()
            && id != m->stkExtrapolOpts->currentIndex()) {
        m->stkExtrapolOpts->setCurrentIndex(id);
        emit extrapolatorNameChanged(extrapolatorName());
    }
}


} // namespace HeightMap
