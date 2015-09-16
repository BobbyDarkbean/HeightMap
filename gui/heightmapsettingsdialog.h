#ifndef _HeightMapSettingsDialog_h_
#define _HeightMapSettingsDialog_h_


#include <QDialog>
#include "hm_shared.h"


namespace HeightMap {


class Preferences;
struct HeightMapSettingsDialogImplementation;
class HeightMapSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HeightMapSettingsDialog(QWidget *parent = 0);

    QSize sizeHint() const;

    const Preferences &preferences() const;
    void setPreferences(const Preferences &);

    ~HeightMapSettingsDialog();

public slots:
    void done(int);

private:
    DISABLE_COPY(HeightMapSettingsDialog)
    DISABLE_MOVE(HeightMapSettingsDialog)

    HeightMapSettingsDialogImplementation *m;
};


} // namespace HeightMap


#endif // _HeightMapSettingsDialog_h_
