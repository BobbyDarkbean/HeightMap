#include <QPushButton>
#include <QFrame>
#include <QBoxLayout>
#include "../preferences.h"
#include "../widgets/peakoptionswidget.h"

#include "generatingoptionsdialog.h"


namespace HeightMap {


struct GeneratingOptionsDialogImplementation
{
    GeneratingOptionsDialogImplementation();

    void adjustControls();
    void adjustLayout(QDialog *master);

    void adjustPreferences();
    void acquirePreferences();

    ~GeneratingOptionsDialogImplementation();

    PeakOptionsWidget *wgtPeakOpt;

    QPushButton *btnOk;
    QPushButton *btnCancel;

    Preferences prefs;

private:
    DISABLE_COPY(GeneratingOptionsDialogImplementation)
    DISABLE_MOVE(GeneratingOptionsDialogImplementation)
};


GeneratingOptionsDialogImplementation::GeneratingOptionsDialogImplementation()
    : wgtPeakOpt(new PeakOptionsWidget),
      btnOk(new QPushButton),
      btnCancel(new QPushButton),
      prefs() { }

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

void GeneratingOptionsDialogImplementation::adjustPreferences()
{
    wgtPeakOpt->setRange(prefs.minPeak(), prefs.maxPeak());
    wgtPeakOpt->setPeakCount(prefs.peakCount());
}

void GeneratingOptionsDialogImplementation::acquirePreferences()
{
    prefs.setMinPeak(wgtPeakOpt->minPeak());
    prefs.setMaxPeak(wgtPeakOpt->maxPeak());
    prefs.setPeakCount(wgtPeakOpt->peakCount());
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

unsigned int GeneratingOptionsDialog::peakCount() const
{ return m->wgtPeakOpt->peakCount(); }

void GeneratingOptionsDialog::setPeakCount(unsigned int value)
{ m->wgtPeakOpt->setPeakCount(value); }

const Preferences &GeneratingOptionsDialog::preferences() const
{ return m->prefs; }

void GeneratingOptionsDialog::setPreferences(const Preferences &prefs)
{
    m->prefs = prefs;
    m->adjustPreferences();
}


GeneratingOptionsDialog::~GeneratingOptionsDialog()
{
    delete m;
}


void GeneratingOptionsDialog::done(int r)
{
    if (r == Accepted)
        m->acquirePreferences();

    QDialog::done(r);
}


} // namespace HeightMap
