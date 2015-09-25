#ifndef _SlopeExtrapolationWidget_h_
#define _SlopeExtrapolationWidget_h_


#include "abstractextrapolationwidget.h"


namespace HeightMap {


class SlopeExtrapolator;
struct SlopeExtrapolationWidgetImplementation;
class SlopeExtrapolationWidget : public AbstractExtrapolationWidget
{
    Q_OBJECT

public:
    explicit SlopeExtrapolationWidget(QWidget *parent = 0);

    Extrapolator *extrapolator() const;
    void bindExtrapolator(SlopeExtrapolator *);

    ~SlopeExtrapolationWidget();

private:
    DISABLE_COPY(SlopeExtrapolationWidget)
    DISABLE_MOVE(SlopeExtrapolationWidget)

    SlopeExtrapolationWidgetImplementation *m;
};


} // namespace HeightMap


#endif // _SlopeExtrapolationWidget_h_
