#ifndef _BaseLevelExtrapolationWidget_h_
#define _BaseLevelExtrapolationWidget_h_


#include "abstractextrapolationwidget.h"


namespace HeightMap {


class BaseLevelExtrapolator;
struct BaseLevelExtrapolationWidgetImplementation;
class BaseLevelExtrapolationWidget : public AbstractExtrapolationWidget
{
    Q_OBJECT

public:
    explicit BaseLevelExtrapolationWidget(QWidget *parent = 0);

    Extrapolator *extrapolator() const;
    void bindExtrapolator(BaseLevelExtrapolator *);

    ~BaseLevelExtrapolationWidget();

private slots:
    void setBaseLevel(double);

private:
    DISABLE_COPY(BaseLevelExtrapolationWidget)
    DISABLE_MOVE(BaseLevelExtrapolationWidget)

    BaseLevelExtrapolationWidgetImplementation *m;
};


} // namespace HeightMap


#endif // _BaseLevelExtrapolationWidget_h_
