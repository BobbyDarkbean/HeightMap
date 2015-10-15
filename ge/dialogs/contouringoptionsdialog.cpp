#include <QPushButton>
#include <QFrame>
#include <QBoxLayout>
#include "preferences.h"
#include "preferencescontroller.h"
#include "../widgets/contouringoptionswidget.h"

#include "contouringoptionsdialog.h"


namespace HeightMap {


struct ContouringOptionsDialogImplementation
{
    ContouringOptionsDialogImplementation();

    void adjustControls();
    void adjustLayout(QDialog *master);

    void adjustPreferences();

    ~ContouringOptionsDialogImplementation();

    ContouringOptionsWidget *wgtContourOpt;

    QPushButton *btnOk;
    QPushButton *btnCancel;

    PreferencesController *ctrl;

private:
    DISABLE_COPY(ContouringOptionsDialogImplementation)
    DISABLE_MOVE(ContouringOptionsDialogImplementation)
};


ContouringOptionsDialogImplementation::ContouringOptionsDialogImplementation()
    : wgtContourOpt(new ContouringOptionsWidget),
      btnOk(new QPushButton),
      btnCancel(new QPushButton),
      ctrl(nullptr) { }

void ContouringOptionsDialogImplementation::adjustControls()
{
    // Buttons
    btnOk->setText(QObject::tr("OK"));
    btnCancel->setText(QObject::tr("Cancel"));
}

void ContouringOptionsDialogImplementation::adjustLayout(QDialog *master)
{
    QFrame *frmButtons = new QFrame(master);

    QBoxLayout *lytButtons = new QHBoxLayout(frmButtons);
    lytButtons->addStretch();
    lytButtons->addWidget(btnOk);
    lytButtons->addWidget(btnCancel);

    QBoxLayout *lytMain = new QVBoxLayout(master);
    lytMain->addWidget(wgtContourOpt);
    lytMain->addWidget(frmButtons);

    ContouringOptionsDialog::connect(btnOk, SIGNAL(clicked()), master, SLOT(accept()));
    ContouringOptionsDialog::connect(btnCancel, SIGNAL(clicked()), master, SLOT(reject()));
}

void ContouringOptionsDialogImplementation::adjustPreferences()
{
    Preferences prefs = *ctrl->preferences();
    wgtContourOpt->setLevelRange(prefs.minContouringLevel(), prefs.maxContouringLevel());
    wgtContourOpt->setStep(prefs.contouringStep());

    ContouringOptionsDialog::connect(wgtContourOpt, SIGNAL(minLevelChanged(int)), ctrl, SLOT(setMinContouringLevel(int)));
    ContouringOptionsDialog::connect(wgtContourOpt, SIGNAL(maxLevelChanged(int)), ctrl, SLOT(setMaxContouringLevel(int)));
    ContouringOptionsDialog::connect(wgtContourOpt, SIGNAL(stepChanged(int)), ctrl, SLOT(setContouringStep(int)));
}

ContouringOptionsDialogImplementation::~ContouringOptionsDialogImplementation() { }


ContouringOptionsDialog::ContouringOptionsDialog(QWidget *parent)
    : QDialog(parent),
      m(new ContouringOptionsDialogImplementation)
{
    m->adjustControls();
    m->adjustLayout(this);
}


int ContouringOptionsDialog::minLevel() const
{ return m->wgtContourOpt->minLevel(); }

int ContouringOptionsDialog::maxLevel() const
{ return m->wgtContourOpt->maxLevel(); }

void ContouringOptionsDialog::setLevelRange(int minValue, int maxValue)
{ m->wgtContourOpt->setLevelRange(minValue, maxValue); }

int ContouringOptionsDialog::step() const
{ return m->wgtContourOpt->step(); }

void ContouringOptionsDialog::setStep(int value)
{ m->wgtContourOpt->setStep(value); }

PreferencesController *ContouringOptionsDialog::preferencesController() const
{ return m->ctrl; }

void ContouringOptionsDialog::setPreferencesController(PreferencesController *ctrl)
{
    m->ctrl = ctrl;
    m->adjustPreferences();
}


ContouringOptionsDialog::~ContouringOptionsDialog()
{
    delete m;
}


} // namespace HeightMap
