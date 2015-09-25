#include "extrapolator.h"
#include "baselevelextrapolationwidget.h"


namespace HeightMap {


struct BaseLevelExtrapolationWidgetImplementation
{
    BaseLevelExtrapolationWidgetImplementation();
    ~BaseLevelExtrapolationWidgetImplementation();

    BaseLevelExtrapolator *x;

private:
    DISABLE_COPY(BaseLevelExtrapolationWidgetImplementation)
    DISABLE_MOVE(BaseLevelExtrapolationWidgetImplementation)
};


BaseLevelExtrapolationWidgetImplementation::BaseLevelExtrapolationWidgetImplementation()
    : x(nullptr) { }

BaseLevelExtrapolationWidgetImplementation::~BaseLevelExtrapolationWidgetImplementation() { }


BaseLevelExtrapolationWidget::BaseLevelExtrapolationWidget(QWidget *parent)
    : AbstractExtrapolationWidget(parent),
      m(new BaseLevelExtrapolationWidgetImplementation) { }


Extrapolator *BaseLevelExtrapolationWidget::extrapolator() const
{ return m->x; }

void BaseLevelExtrapolationWidget::bindExtrapolator(BaseLevelExtrapolator *blx)
{ m->x = blx; }


BaseLevelExtrapolationWidget::~BaseLevelExtrapolationWidget()
{
    delete m;
}


} // namespace HeightMap
