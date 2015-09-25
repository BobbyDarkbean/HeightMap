#ifndef _FixedRadiusExtrapolationWidget_h_
#define _FixedRadiusExtrapolationWidget_h_


#include "abstractextrapolationwidget.h"


namespace HeightMap {


class FixedRadiusExtrapolator;
struct FixedRadiusExtrapolationWidgetImplementation;
class FixedRadiusExtrapolationWidget : public AbstractExtrapolationWidget
{
    Q_OBJECT

public:
    explicit FixedRadiusExtrapolationWidget(QWidget *parent = 0);

    Extrapolator *extrapolator() const;
    void bindExtrapolator(FixedRadiusExtrapolator *);

    ~FixedRadiusExtrapolationWidget();

private:
    DISABLE_COPY(FixedRadiusExtrapolationWidget)
    DISABLE_MOVE(FixedRadiusExtrapolationWidget)

    FixedRadiusExtrapolationWidgetImplementation *m;
};


} // namespace HeightMap


#endif // _FixedRadiusExtrapolationWidget_h_
