#include <QPushButton>
#include <QFrame>
#include <QBoxLayout>
#include "preferences.h"
#include "../preferencescontroller.h"
#include "../widgets/peakoptionswidget.h"

#include "generatingoptionsdialog.h"


namespace HeightMap {


struct GeneratingOptionsDialogImplementation
{
    GeneratingOptionsDialogImplementation();

    void adjustControls();
    void adjustLayout(QDialog *master);

    void adjustPreferences();

    ~GeneratingOptionsDialogImplementation();

    PeakOptionsWidget *wgtPeakOpt;

    QPushButton *btnOk;
    QPushButton *btnCancel;

    PreferencesController *ctrl;

private:
    DISABLE_COPY(GeneratingOptionsDialogImplementation)
    DISABLE_MOVE(GeneratingOptionsDialogImplementation)
};


GeneratingOptionsDialogImplementation::GeneratingOptionsDialogImplementation()
    : wgtPeakOpt(new PeakOptionsWidget),
      btnOk(new QPushButton),
      btnCancel(new QPushButton),
      ctrl(nullptr) { }

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
    Preferences prefs = *ctrl->preferences();
    wgtPeakOpt->setRange(prefs.minPeak(), prefs.maxPeak());
    wgtPeakOpt->setPeakCount(prefs.peakCount());

    GeneratingOptionsDialog::connect(wgtPeakOpt, SIGNAL(minPeakChanged(int)), ctrl, SLOT(setMinPeak(int)));
    GeneratingOptionsDialog::connect(wgtPeakOpt, SIGNAL(maxPeakChanged(int)), ctrl, SLOT(setMaxPeak(int)));
    GeneratingOptionsDialog::connect(wgtPeakOpt, SIGNAL(peakCountChanged(int)), ctrl, SLOT(setPeakCount(int)));
}

GeneratingOptionsDialogImplementation::~GeneratingOptionsDialogImplementation() { }


GeneratingOptionsDialog::GeneratingOptionsDialog(QWidget *parent)
    : QDialog(parent),
      m(new GeneratingOptionsDialogImplementation)
{
    m->adjustControls();
    m->adjustLayout(this);
}


QSize GeneratingOptionsDialog::sizeHint() const
{
    return QSize(540, 120);
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

PreferencesController *GeneratingOptionsDialog::preferencesController() const
{ return m->ctrl; }

void GeneratingOptionsDialog::setPreferencesController(PreferencesController *ctrl)
{
    m->ctrl = ctrl;
    m->adjustPreferences();
}


GeneratingOptionsDialog::~GeneratingOptionsDialog()
{
    delete m;
}


} // namespace HeightMap
