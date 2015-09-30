#ifndef _ExtrapolationOptionsDialog_h_
#define _ExtrapolationOptionsDialog_h_


#include <QDialog>
#include "hm_shared.h"


namespace HeightMap {


class PreferencesController;
class AbstractExtrapolationWidget;
struct ExtrapolationOptionsDialogImplementation;
class ExtrapolationOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExtrapolationOptionsDialog(QWidget *parent = 0);

    PreferencesController *preferencesController() const;
    void setPreferencesController(PreferencesController *);

    QWidget *extrapolationWidget(const QString &keyName) const;
    void addExtrapolationWidget(
        const QString &keyName,
        const QString &description,
        AbstractExtrapolationWidget *);

    ~ExtrapolationOptionsDialog();

public slots:
    void done(int);

private:
    DISABLE_COPY(ExtrapolationOptionsDialog)
    DISABLE_MOVE(ExtrapolationOptionsDialog)

    ExtrapolationOptionsDialogImplementation *m;
};


} // namespace HeightMap


#endif // _ExtrapolationOptionsDialog_h_
