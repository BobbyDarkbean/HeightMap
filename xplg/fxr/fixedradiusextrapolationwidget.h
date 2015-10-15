#ifndef _FixedRadiusExtrapolationWidget_h_
#define _FixedRadiusExtrapolationWidget_h_


#include "widgets/abstractextrapolationwidget.h"


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

    void refreshData();

    ExtrapolationData extractData() const;
    void provideData(const ExtrapolationData &);

    ~FixedRadiusExtrapolationWidget();

private slots:
    void setBaseLevel(double);
    void setFixedRadius(int);

private:
    DISABLE_COPY(FixedRadiusExtrapolationWidget)
    DISABLE_MOVE(FixedRadiusExtrapolationWidget)

    FixedRadiusExtrapolationWidgetImplementation *m;
};


} // namespace HeightMap


#endif // _FixedRadiusExtrapolationWidget_h_
