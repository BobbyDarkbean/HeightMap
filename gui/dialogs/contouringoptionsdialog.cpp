#include <QPushButton>
#include <QFrame>
#include <QBoxLayout>
#include "../preferences.h"
#include "../widgets/contouringoptionswidget.h"

#include "contouringoptionsdialog.h"


namespace HeightMap {


struct ContouringOptionsDialogImplementation
{
    ContouringOptionsDialogImplementation();

    void adjustControls();
    void adjustLayout(QDialog *master);

    void adjustPreferences();
    void acquirePreferences();

    ~ContouringOptionsDialogImplementation();

    ContouringOptionsWidget *wgtContourOpt;

    QPushButton *btnOk;
    QPushButton *btnCancel;

    Preferences prefs;

private:
    DISABLE_COPY(ContouringOptionsDialogImplementation)
    DISABLE_MOVE(ContouringOptionsDialogImplementation)
};


ContouringOptionsDialogImplementation::ContouringOptionsDialogImplementation()
    : wgtContourOpt(new ContouringOptionsWidget),
      btnOk(new QPushButton),
      btnCancel(new QPushButton),
      prefs() { }

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
    wgtContourOpt->setLevelRange(prefs.minContouringLevel(), prefs.maxContouringLevel());
    wgtContourOpt->setStep(prefs.contouringStep());
}

void ContouringOptionsDialogImplementation::acquirePreferences()
{
    prefs.setMinContouringLevel(wgtContourOpt->minLevel());
    prefs.setMaxContouringLevel(wgtContourOpt->maxLevel());
    prefs.setContouringStep(wgtContourOpt->step());
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

const Preferences &ContouringOptionsDialog::preferences() const
{ return m->prefs; }

void ContouringOptionsDialog::setPreferences(const Preferences &prefs)
{
    m->prefs = prefs;
    m->adjustPreferences();
}


ContouringOptionsDialog::~ContouringOptionsDialog()
{
    delete m;
}


void ContouringOptionsDialog::done(int r)
{
    if (r == Accepted)
        m->acquirePreferences();

    QDialog::done(r);
}


} // namespace HeightMap
