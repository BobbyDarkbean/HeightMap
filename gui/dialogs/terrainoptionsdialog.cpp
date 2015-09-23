#include <QPushButton>
#include <QFrame>
#include <QBoxLayout>
#include "../preferences.h"
#include "../widgets/landscapeoptionswidget.h"

#include "terrainoptionsdialog.h"


namespace HeightMap {


struct TerrainOptionsDialogImplementation
{
    TerrainOptionsDialogImplementation();

    void adjustControls();
    void adjustLayout(QDialog *master);

    void adjustPreferences();
    void acquirePreferences();

    ~TerrainOptionsDialogImplementation();

    LandscapeOptionsWidget *wgtLsOpt;

    QPushButton *btnOk;
    QPushButton *btnCancel;

    Preferences prefs;

private:
    DISABLE_COPY(TerrainOptionsDialogImplementation)
    DISABLE_MOVE(TerrainOptionsDialogImplementation)
};


TerrainOptionsDialogImplementation::TerrainOptionsDialogImplementation()
    : wgtLsOpt(new LandscapeOptionsWidget),
      btnOk(new QPushButton),
      btnCancel(new QPushButton),
      prefs() { }

void TerrainOptionsDialogImplementation::adjustControls()
{
    // Buttons
    btnOk->setText(QObject::tr("OK"));
    btnCancel->setText(QObject::tr("Cancel"));
}

void TerrainOptionsDialogImplementation::adjustLayout(QDialog *master)
{
    QFrame *frmButtons = new QFrame(master);

    QBoxLayout *lytButtons = new QHBoxLayout(frmButtons);
    lytButtons->addStretch();
    lytButtons->addWidget(btnOk);
    lytButtons->addWidget(btnCancel);

    QBoxLayout *lytMain = new QVBoxLayout(master);
    lytMain->addWidget(wgtLsOpt);
    lytMain->addWidget(frmButtons);

    TerrainOptionsDialog::connect(btnOk, SIGNAL(clicked()), master, SLOT(accept()));
    TerrainOptionsDialog::connect(btnCancel, SIGNAL(clicked()), master, SLOT(reject()));
}

void TerrainOptionsDialogImplementation::adjustPreferences()
{
    wgtLsOpt->setLandscapeWidth(prefs.landscapeWidth());
    wgtLsOpt->setLandscapeHeight(prefs.landscapeHeight());
}

void TerrainOptionsDialogImplementation::acquirePreferences()
{
    prefs.setLandscapeWidth(wgtLsOpt->landscapeWidth());
    prefs.setLandscapeHeight(wgtLsOpt->landscapeHeight());
}

TerrainOptionsDialogImplementation::~TerrainOptionsDialogImplementation() { }


TerrainOptionsDialog::TerrainOptionsDialog(QWidget *parent)
    : QDialog(parent),
      m(new TerrainOptionsDialogImplementation)
{
    m->adjustControls();
    m->adjustLayout(this);
}


int TerrainOptionsDialog::landscapeWidth() const
{ return m->wgtLsOpt->landscapeWidth(); }

void TerrainOptionsDialog::setLandscapeWidth(int w)
{ m->wgtLsOpt->setLandscapeWidth(w); }

int TerrainOptionsDialog::landscapeHeight() const
{ return m->wgtLsOpt->landscapeHeight(); }

void TerrainOptionsDialog::setLandscapeHeight(int h)
{ m->wgtLsOpt->setLandscapeHeight(h); }

const Preferences &TerrainOptionsDialog::preferences() const
{ return m->prefs; }

void TerrainOptionsDialog::setPreferences(const Preferences &prefs)
{
    m->prefs = prefs;
    m->adjustPreferences();
}


TerrainOptionsDialog::~TerrainOptionsDialog()
{
    delete m;
}


void TerrainOptionsDialog::done(int r)
{
    if (r == Accepted)
        m->acquirePreferences();

    QDialog::done(r);
}


} // namespace HeightMap
