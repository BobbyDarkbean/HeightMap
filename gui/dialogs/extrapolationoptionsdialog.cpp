#include <QPushButton>
#include <QFrame>
#include <QBoxLayout>
#include "../widgets/extrapolationoptionswidget.h"

#include "extrapolationoptionsdialog.h"


namespace HeightMap {


struct ExtrapolationOptionsDialogImplementation
{
    ExtrapolationOptionsDialogImplementation();

    void adjustControls();
    void adjustLayout(QDialog *master);

    ~ExtrapolationOptionsDialogImplementation();

    ExtrapolationOptionsWidget *wgtExtrapolOpt;

    QPushButton *btnOk;
    QPushButton *btnCancel;

private:
    DISABLE_COPY(ExtrapolationOptionsDialogImplementation)
    DISABLE_MOVE(ExtrapolationOptionsDialogImplementation)
};


ExtrapolationOptionsDialogImplementation::ExtrapolationOptionsDialogImplementation()
    : wgtExtrapolOpt(new ExtrapolationOptionsWidget),
      btnOk(new QPushButton),
      btnCancel(new QPushButton) { }

void ExtrapolationOptionsDialogImplementation::adjustControls()
{
    // Buttons
    btnOk->setText(QObject::tr("OK"));
    btnCancel->setText(QObject::tr("Cancel"));
}

void ExtrapolationOptionsDialogImplementation::adjustLayout(QDialog *master)
{
    QFrame *frmButtons = new QFrame(master);

    QBoxLayout *lytButtons = new QHBoxLayout(frmButtons);
    lytButtons->addStretch();
    lytButtons->addWidget(btnOk);
    lytButtons->addWidget(btnCancel);

    QBoxLayout *lytMain = new QVBoxLayout(master);
    lytMain->addWidget(wgtExtrapolOpt);
    lytMain->addWidget(frmButtons);

    ExtrapolationOptionsDialog::connect(btnOk, SIGNAL(clicked()), master, SLOT(accept()));
    ExtrapolationOptionsDialog::connect(btnCancel, SIGNAL(clicked()), master, SLOT(reject()));
}

ExtrapolationOptionsDialogImplementation::~ExtrapolationOptionsDialogImplementation() { }


ExtrapolationOptionsDialog::ExtrapolationOptionsDialog(QWidget *parent)
    : QDialog(parent),
      m(new ExtrapolationOptionsDialogImplementation)
{
    m->adjustControls();
    m->adjustLayout(this);
}


int ExtrapolationOptionsDialog::baseLevel() const
{ return m->wgtExtrapolOpt->baseLevel(); }

void ExtrapolationOptionsDialog::setBaseLevel(int level)
{ m->wgtExtrapolOpt->setBaseLevel(level); }


ExtrapolationOptionsDialog::~ExtrapolationOptionsDialog()
{
    delete m;
}


} // namespace HeightMap
