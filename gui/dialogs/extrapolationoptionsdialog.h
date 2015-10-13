#ifndef _ExtrapolationOptionsDialog_h_
#define _ExtrapolationOptionsDialog_h_


#include <QDialog>
#include "hm_shared.h"


namespace HeightMap {


class PreferencesController;
class ExtrapolationData;
class ExtrapolationFactory;
struct ExtrapolationOptionsDialogImplementation;
class ExtrapolationOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExtrapolationOptionsDialog(QWidget *parent = 0);

    QString extrapolatorName() const;
    void setExtrapolatorName(const QString &);

    PreferencesController *preferencesController() const;
    void setPreferencesController(PreferencesController *);

    ExtrapolationData xData();

    QWidget *extrapolationWidget(const QString &keyName) const;
    void addExtrapolationWidget(ExtrapolationFactory *);

    ~ExtrapolationOptionsDialog();

private:
    DISABLE_COPY(ExtrapolationOptionsDialog)
    DISABLE_MOVE(ExtrapolationOptionsDialog)

    ExtrapolationOptionsDialogImplementation *m;
};


} // namespace HeightMap


#endif // _ExtrapolationOptionsDialog_h_
