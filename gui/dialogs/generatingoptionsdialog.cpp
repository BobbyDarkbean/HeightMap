#include <QPushButton>
#include <QFrame>
#include <QBoxLayout>
#include "../widgets/peakoptionswidget.h"

#include "generatingoptionsdialog.h"


namespace HeightMap {


struct GeneratingOptionsDialogImplementation
{
    GeneratingOptionsDialogImplementation();

    void adjustControls();
    void adjustLayout(QDialog *master);

    ~GeneratingOptionsDialogImplementation();

    PeakOptionsWidget *wgtPeakOpt;

    QPushButton *btnOk;
    QPushButton *btnCancel;

private:
    DISABLE_COPY(GeneratingOptionsDialogImplementation)
    DISABLE_MOVE(GeneratingOptionsDialogImplementation)
};


GeneratingOptionsDialogImplementation::GeneratingOptionsDialogImplementation()
    : wgtPeakOpt(new PeakOptionsWidget),
      btnOk(new QPushButton),
      btnCancel(new QPushButton) { }

void GeneratingOptionsDialogImplementation::adjustControls()
{
    // Buttons
    btnOk->setText(QObject::tr("OK"));
    btnCancel->setText(QObject::tr("Cancel"));
}

void GeneratingOptionsDialogImplementation::adjustLayout(QDialog *master)
{
    QFrame *frmButtons = new QFrame(master);

    QBoxLayout *lytButtons = new QHBoxLayout(frmButtons);
    lytButtons->addStretch();
    lytButtons->addWidget(btnOk);
    lytButtons->addWidget(btnCancel);

    QBoxLayout *lytMain = new QVBoxLayout(master);
    lytMain->addWidget(wgtPeakOpt);
    lytMain->addWidget(frmButtons);

    GeneratingOptionsDialog::connect(btnOk, SIGNAL(clicked()), master, SLOT(accept()));
    GeneratingOptionsDialog::connect(btnCancel, SIGNAL(clicked()), master, SLOT(reject()));
}

GeneratingOptionsDialogImplementation::~GeneratingOptionsDialogImplementation() { }


GeneratingOptionsDialog::GeneratingOptionsDialog(QWidget *parent)
    : QDialog(parent),
      m(new GeneratingOptionsDialogImplementation)
{
    m->adjustControls();
    m->adjustLayout(this);
}


int GeneratingOptionsDialog::minPeak() const
{ return m->wgtPeakOpt->minPeak(); }

int GeneratingOptionsDialog::maxPeak() const
{ return m->wgtPeakOpt->maxPeak(); }

void GeneratingOptionsDialog::setRange(int minValue, int maxValue)
{ m->wgtPeakOpt->setRange(minValue, maxValue); }

int GeneratingOptionsDialog::peakCount() const
{ return m->wgtPeakOpt->peakCount(); }

void GeneratingOptionsDialog::setPeakCount(int value)
{ m->wgtPeakOpt->setPeakCount(value); }


GeneratingOptionsDialog::~GeneratingOptionsDialog()
{
    delete m;
}


} // namespace HeightMap
