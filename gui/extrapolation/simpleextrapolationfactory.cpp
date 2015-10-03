#include "../widgets/simpleextrapolationwidget.h"
#include "extrapolator.h"
#include "xreader.h"
#include "xwriter.h"

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
    XReader xr("xdata/sml.xml");
    x->setBaseLevel(xr.readElement("baselevel", -1.0));
}

SimpleExtrapolationFactoryImplementation::~SimpleExtrapolationFactoryImplementation()
{
    XWriter xw("xdata/sml.xml");
    xw.writeElement("baselevel", x->baseLevel());

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
