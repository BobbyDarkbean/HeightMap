#ifndef _SimpleExtrapolationWidget_h_
#define _SimpleExtrapolationWidget_h_


#include "widgets/abstractextrapolationwidget.h"


namespace HeightMap {


class SimpleExtrapolator;
struct SimpleExtrapolationWidgetImplementation;
class SimpleExtrapolationWidget : public AbstractExtrapolationWidget
{
    Q_OBJECT

public:
    explicit SimpleExtrapolationWidget(QWidget *parent = 0);

    Extrapolator *extrapolator() const;
    void bindExtrapolator(SimpleExtrapolator *);

    void refreshData();

    ExtrapolationData extractData() const;
    void provideData(const ExtrapolationData &);

    ~SimpleExtrapolationWidget();

private slots:
    void setBaseLevel(double);

private:
    DISABLE_COPY(SimpleExtrapolationWidget)
    DISABLE_MOVE(SimpleExtrapolationWidget)

    SimpleExtrapolationWidgetImplementation *m;
};


} // namespace HeightMap


#endif // _SimpleExtrapolationWidget_h_