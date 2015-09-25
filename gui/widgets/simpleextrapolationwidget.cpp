#include "extrapolator.h"
#include "simpleextrapolationwidget.h"


namespace HeightMap {


struct SimpleExtrapolationWidgetImplementation
{
    SimpleExtrapolationWidgetImplementation();
    ~SimpleExtrapolationWidgetImplementation();

    SimpleExtrapolator *x;

private:
    DISABLE_COPY(SimpleExtrapolationWidgetImplementation)
    DISABLE_MOVE(SimpleExtrapolationWidgetImplementation)
};


SimpleExtrapolationWidgetImplementation::SimpleExtrapolationWidgetImplementation()
    : x(nullptr) { }

SimpleExtrapolationWidgetImplementation::~SimpleExtrapolationWidgetImplementation() { }


SimpleExtrapolationWidget::SimpleExtrapolationWidget(QWidget *parent)
    : AbstractExtrapolationWidget(parent),
      m(new SimpleExtrapolationWidgetImplementation) { }


Extrapolator *SimpleExtrapolationWidget::extrapolator() const
{ return m->x; }

void SimpleExtrapolationWidget::bindExtrapolator(SimpleExtrapolator *sx)
{ m->x = sx; }


SimpleExtrapolationWidget::~SimpleExtrapolationWidget()
{
    delete m;
}


} // namespace HeightMap
