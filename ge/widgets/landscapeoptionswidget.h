#ifndef _LandscapeOptionsWidget_h_
#define _LandscapeOptionsWidget_h_


#include <QWidget>
#include "hmdef.h"
#include "../ge_shared.h"


namespace HeightMap {


struct LandscapeOptionsWidgetImplementation;
class HMGE_SHARED LandscapeOptionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LandscapeOptionsWidget(QWidget *parent = 0);

    int landscapeWidth() const;
    void setLandscapeWidth(int);

    int landscapeHeight() const;
    void setLandscapeHeight(int);

    ~LandscapeOptionsWidget();

private:
    DISABLE_COPY(LandscapeOptionsWidget)
    DISABLE_MOVE(LandscapeOptionsWidget)

    LandscapeOptionsWidgetImplementation *m;
};


} // namespace HeightMap


#endif // _LandscapeOptionsWidget_h_
