#include <QPushButton>
#include <QFrame>
#include <QBoxLayout>
#include "../preferences.h"
#include "../widgets/extrapolationoptionswidget.h"

#include "extrapolationoptionsdialog.h"


namespace HeightMap {


struct ExtrapolationOptionsDialogImplementation
{
    ExtrapolationOptionsDialogImplementation();

    void adjustControls();
    void adjustLayout(QDialog *master);

    void adjustPreferences();
    void acquirePreferences();

    ~ExtrapolationOptionsDialogImplementation();

    ExtrapolationOptionsWidget *wgtExtrapolOpt;

    QPushButton *btnOk;
    QPushButton *btnCancel;

    Preferences prefs;

private:
    DISABLE_COPY(ExtrapolationOptionsDialogImplementation)
    DISABLE_MOVE(ExtrapolationOptionsDialogImplementation)
};


ExtrapolationOptionsDialogImplementation::ExtrapolationOptionsDialogImplementation()
    : wgtExtrapolOpt(new ExtrapolationOptionsWidget),
      btnOk(new QPushButton),
      btnCancel(new QPushButton),
      prefs() { }

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

void ExtrapolationOptionsDialogImplementation::adjustPreferences()
{
    wgtExtrapolOpt->setExtrapolatorName(prefs.extrapolatorName());
}

void ExtrapolationOptionsDialogImplementation::acquirePreferences()
{
    prefs.setExtrapolatorName(wgtExtrapolOpt->extrapolatorName());
}

ExtrapolationOptionsDialogImplementation::~ExtrapolationOptionsDialogImplementation() { }


ExtrapolationOptionsDialog::ExtrapolationOptionsDialog(QWidget *parent)
    : QDialog(parent),
      m(new ExtrapolationOptionsDialogImplementation)
{
    m->adjustControls();
    m->adjustLayout(this);
}


const Preferences &ExtrapolationOptionsDialog::preferences() const
{ return m->prefs; }

void ExtrapolationOptionsDialog::setPreferences(const Preferences &prefs)
{
    m->prefs = prefs;
    m->adjustPreferences();
}

QWidget *ExtrapolationOptionsDialog::extrapolationWidget(const QString &keyName) const
{ return m->wgtExtrapolOpt->extrapolationWidget(keyName); }

void ExtrapolationOptionsDialog::addExtrapolationWidget(
    const QString &keyName,
    const QString &description,
    AbstractExtrapolationWidget *w)
{ m->wgtExtrapolOpt->addExtrapolationWidget(keyName, description, w); }


ExtrapolationOptionsDialog::~ExtrapolationOptionsDialog()
{
    delete m;
}


void ExtrapolationOptionsDialog::done(int r)
{
    if (r == Accepted) {
        m->wgtExtrapolOpt->acceptExtrapolationSettings();
        m->acquirePreferences();
    }

    QDialog::done(r);
}


} // namespace HeightMap
