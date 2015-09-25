#include "extrapolator.h"
#include "slopeextrapolationwidget.h"


namespace HeightMap {


struct SlopeExtrapolationWidgetImplementation
{
    SlopeExtrapolationWidgetImplementation();
    ~SlopeExtrapolationWidgetImplementation();

    SlopeExtrapolator *x;

private:
    DISABLE_COPY(SlopeExtrapolationWidgetImplementation)
    DISABLE_MOVE(SlopeExtrapolationWidgetImplementation)
};


SlopeExtrapolationWidgetImplementation::SlopeExtrapolationWidgetImplementation()
    : x(nullptr) { }

SlopeExtrapolationWidgetImplementation::~SlopeExtrapolationWidgetImplementation() { }


SlopeExtrapolationWidget::SlopeExtrapolationWidget(QWidget *parent)
    : AbstractExtrapolationWidget(parent),
      m(new SlopeExtrapolationWidgetImplementation) { }


Extrapolator *SlopeExtrapolationWidget::extrapolator() const
{ return m->x; }

void SlopeExtrapolationWidget::bindExtrapolator(SlopeExtrapolator *sx)
{ m->x = sx; }


SlopeExtrapolationWidget::~SlopeExtrapolationWidget()
{
    delete m;
}


} // namespace HeightMap
