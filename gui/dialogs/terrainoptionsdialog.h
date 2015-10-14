#ifndef _TerrainOptionsDialog_h_
#define _TerrainOptionsDialog_h_


#include <QDialog>
#include "hmdef.h"


namespace HeightMap {


class Preferences;
struct TerrainOptionsDialogImplementation;
class TerrainOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TerrainOptionsDialog(QWidget *parent = 0);

    int landscapeWidth() const;
    void setLandscapeWidth(int);

    int landscapeHeight() const;
    void setLandscapeHeight(int);

    const Preferences &preferences() const;
    void setPreferences(const Preferences &);

    ~TerrainOptionsDialog();

public slots:
    void done(int);

private:
    DISABLE_COPY(TerrainOptionsDialog)
    DISABLE_MOVE(TerrainOptionsDialog)

    TerrainOptionsDialogImplementation *m;
};


} // namespace HeightMap


#endif // _TerrainOptionsDialog_h_
