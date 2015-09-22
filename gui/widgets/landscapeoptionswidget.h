#ifndef _LandscapeOptionsWidget_h_
#define _LandscapeOptionsWidget_h_


#include "hm_shared.h"
#include <QWidget>


namespace HeightMap {


struct LandscapeOptionsWidgetImplementation;
class LandscapeOptionsWidget : public QWidget
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
