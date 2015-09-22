#ifndef _TerrainOptionsDialog_h_
#define _TerrainOptionsDialog_h_


#include "hm_shared.h"
#include <QDialog>


namespace HeightMap {


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

    ~TerrainOptionsDialog();

private:
    DISABLE_COPY(TerrainOptionsDialog)
    DISABLE_MOVE(TerrainOptionsDialog)

    TerrainOptionsDialogImplementation *m;
};


} // namespace HeightMap


#endif // _TerrainOptionsDialog_h_
