#include "../widgets/simpleextrapolationwidget.h"
#include "extrapolator.h"

#include "simpleextrapolationfactory.h"


namespace HeightMap {


struct SimpleExtrapolationFactoryImplementation
{
    SimpleExtrapolationFactoryImplementation();
    ~SimpleExtrapolationFactoryImplementation();

    SimpleExtrapolator *x;

private:
    DISABLE_COPY(SimpleExtrapolationFactoryImplementation)
    DISABLE_MOVE(SimpleExtrapolationFactoryImplementation)
};


SimpleExtrapolationFactoryImplementation::SimpleExtrapolationFactoryImplementation()
    : x(new SimpleExtrapolator)
{
    // read extrapolator settings from xdata/simple.xml
}

SimpleExtrapolationFactoryImplementation::~SimpleExtrapolationFactoryImplementation()
{
    // write extrapolator settings to xdata/simple.xml
    delete x;
}


SimpleExtrapolationFactory::SimpleExtrapolationFactory()
    : ExtrapolationFactory(),
      m(new SimpleExtrapolationFactoryImplementation) { }


Extrapolator *SimpleExtrapolationFactory::extrapolator() const
{ return m->x; }

AbstractExtrapolationWidget *SimpleExtrapolationFactory::createWidget() const
{
    SimpleExtrapolationWidget *widget = new SimpleExtrapolationWidget;
    widget->bindExtrapolator(m->x);
    return widget;
}

QString SimpleExtrapolationFactory::name() const
{ return SimpleExtrapolationWidget::tr("sml"); }

QString SimpleExtrapolationFactory::description() const
{ return SimpleExtrapolationWidget::tr("Simple"); }


SimpleExtrapolationFactory::~SimpleExtrapolationFactory()
{
    delete m;
}


} // namespace HeightMap
