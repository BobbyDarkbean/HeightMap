#include <QPushButton>
#include <QFrame>
#include <QBoxLayout>
#include "../widgets/landscapeoptionswidget.h"

#include "terrainoptionsdialog.h"


namespace HeightMap {


struct TerrainOptionsDialogImplementation
{
    TerrainOptionsDialogImplementation();

    void adjustControls();
    void adjustLayout(QDialog *master);

    ~TerrainOptionsDialogImplementation();

    LandscapeOptionsWidget *wgtLsOpt;

    QPushButton *btnOk;
    QPushButton *btnCancel;

private:
    DISABLE_COPY(TerrainOptionsDialogImplementation)
    DISABLE_MOVE(TerrainOptionsDialogImplementation)
};


TerrainOptionsDialogImplementation::TerrainOptionsDialogImplementation()
    : wgtLsOpt(new LandscapeOptionsWidget),
      btnOk(new QPushButton),
      btnCancel(new QPushButton) { }

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

int TerrainOptionsDialog::landscapeHeight() const
{ return m->wgtLsOpt->landscapeHeight(); }


TerrainOptionsDialog::~TerrainOptionsDialog()
{
    delete m;
}


} // namespace HeightMap
