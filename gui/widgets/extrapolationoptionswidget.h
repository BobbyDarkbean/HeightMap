#ifndef _ExtrapolationOptionsWidget_h_
#define _ExtrapolationOptionsWidget_h_


#include <QWidget>
#include "hm_shared.h"


namespace HeightMap {


struct ExtrapolationOptionsWidgetImplementation;
class ExtrapolationOptionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExtrapolationOptionsWidget(QWidget *parent = 0);

    int baseLevel() const;
    void setBaseLevel(int);

    ~ExtrapolationOptionsWidget();

private:
    DISABLE_COPY(ExtrapolationOptionsWidget)
    DISABLE_MOVE(ExtrapolationOptionsWidget)

    ExtrapolationOptionsWidgetImplementation *m;
};


} // namespace HeightMap


#endif // _ExtrapolationOptionsWidget_h_
