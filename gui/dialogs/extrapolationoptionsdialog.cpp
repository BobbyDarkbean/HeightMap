#include <QPushButton>
#include <QFrame>
#include <QBoxLayout>
#include "../preferences.h"
#include "../preferencescontroller.h"
#include "../widgets/extrapolationoptionswidget.h"

#include "extrapolationoptionsdialog.h"


namespace HeightMap {


struct ExtrapolationOptionsDialogImplementation
{
    ExtrapolationOptionsDialogImplementation();

    void adjustControls();
    void adjustLayout(QDialog *master);

    void adjustPreferences();

    ~ExtrapolationOptionsDialogImplementation();

    ExtrapolationOptionsWidget *wgtExtrapolOpt;

    QPushButton *btnOk;
    QPushButton *btnCancel;

    PreferencesController *ctrl;

private:
    DISABLE_COPY(ExtrapolationOptionsDialogImplementation)
    DISABLE_MOVE(ExtrapolationOptionsDialogImplementation)
};


ExtrapolationOptionsDialogImplementation::ExtrapolationOptionsDialogImplementation()
    : wgtExtrapolOpt(new ExtrapolationOptionsWidget),
      btnOk(new QPushButton),
      btnCancel(new QPushButton),
      ctrl(nullptr) { }

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
    Preferences prefs = *ctrl->preferences();
    wgtExtrapolOpt->setExtrapolatorName(prefs.extrapolatorName());

    ExtrapolationOptionsDialog::connect(
                wgtExtrapolOpt, SIGNAL(extrapolatorNameChanged(QString)), ctrl, SLOT(setExtrapolatorName(QString)));
}

ExtrapolationOptionsDialogImplementation::~ExtrapolationOptionsDialogImplementation() { }


ExtrapolationOptionsDialog::ExtrapolationOptionsDialog(QWidget *parent)
    : QDialog(parent),
      m(new ExtrapolationOptionsDialogImplementation)
{
    m->adjustControls();
    m->adjustLayout(this);
}


PreferencesController *ExtrapolationOptionsDialog::preferencesController() const
{ return m->ctrl; }

void ExtrapolationOptionsDialog::setPreferencesController(PreferencesController *ctrl)
{
    m->ctrl = ctrl;
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


} // namespace HeightMap
