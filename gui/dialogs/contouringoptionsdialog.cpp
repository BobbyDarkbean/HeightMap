#include <QPushButton>
#include <QFrame>
#include <QBoxLayout>
#include "../widgets/contouringoptionswidget.h"

#include "contouringoptionsdialog.h"


namespace HeightMap {


struct ContouringOptionsDialogImplementation
{
    ContouringOptionsDialogImplementation();

    void adjustControls();
    void adjustLayout(QDialog *master);

    ~ContouringOptionsDialogImplementation();

    ContouringOptionsWidget *wgtContourOpt;

    QPushButton *btnOk;
    QPushButton *btnCancel;

private:
    DISABLE_COPY(ContouringOptionsDialogImplementation)
    DISABLE_MOVE(ContouringOptionsDialogImplementation)
};


ContouringOptionsDialogImplementation::ContouringOptionsDialogImplementation()
    : wgtContourOpt(new ContouringOptionsWidget),
      btnOk(new QPushButton),
      btnCancel(new QPushButton) { }

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

void ContouringOptionsDialog::setMinLevel(int level)
{ m->wgtContourOpt->setMinLevel(level); }

int ContouringOptionsDialog::maxLevel() const
{ return m->wgtContourOpt->maxLevel(); }

void ContouringOptionsDialog::setMaxLevel(int level)
{ m->wgtContourOpt->setMaxLevel(level); }

int ContouringOptionsDialog::step() const
{ return m->wgtContourOpt->step(); }

void ContouringOptionsDialog::setStep(int value)
{ m->wgtContourOpt->setStep(value); }


ContouringOptionsDialog::~ContouringOptionsDialog()
{
    delete m;
}


} // namespace HeightMap
