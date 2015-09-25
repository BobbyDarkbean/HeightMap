#include "extrapolator.h"
#include "fixedradiusextrapolationwidget.h"


namespace HeightMap {


struct FixedRadiusExtrapolationWidgetImplementation
{
    FixedRadiusExtrapolationWidgetImplementation();
    ~FixedRadiusExtrapolationWidgetImplementation();

    FixedRadiusExtrapolator *x;

private:
    DISABLE_COPY(FixedRadiusExtrapolationWidgetImplementation)
    DISABLE_MOVE(FixedRadiusExtrapolationWidgetImplementation)
};


FixedRadiusExtrapolationWidgetImplementation::FixedRadiusExtrapolationWidgetImplementation()
    : x(nullptr) { }

FixedRadiusExtrapolationWidgetImplementation::~FixedRadiusExtrapolationWidgetImplementation() { }


FixedRadiusExtrapolationWidget::FixedRadiusExtrapolationWidget(QWidget *parent)
    : AbstractExtrapolationWidget(parent),
      m(new FixedRadiusExtrapolationWidgetImplementation) { }


Extrapolator *FixedRadiusExtrapolationWidget::extrapolator() const
{ return m->x; }

void FixedRadiusExtrapolationWidget::bindExtrapolator(FixedRadiusExtrapolator *frx)
{ m->x = frx; }


FixedRadiusExtrapolationWidget::~FixedRadiusExtrapolationWidget()
{
    delete m;
}


} // namespace HeightMap
