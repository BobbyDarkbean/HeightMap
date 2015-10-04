#include "../widgets/simpleextrapolationwidget.h"
#include "extrapolator.h"
#include "xreader.h"
#include "xwriter.h"

#include "simpleextrapolationfactory.h"


namespace HeightMap {


struct SimpleExtrapolationFactoryImplementation
{
    SimpleExtrapolationFactoryImplementation();

    void applyProxy();
    void resetProxy();

    ~SimpleExtrapolationFactoryImplementation();

    SimpleExtrapolator *x;
    SimpleExtrapolator *proxy;

private:
    DISABLE_COPY(SimpleExtrapolationFactoryImplementation)
    DISABLE_MOVE(SimpleExtrapolationFactoryImplementation)
};


SimpleExtrapolationFactoryImplementation::SimpleExtrapolationFactoryImplementation()
    : x(new SimpleExtrapolator),
      proxy(new SimpleExtrapolator)
{
    XReader xr("xdata/sml.xml");
    x->setBaseLevel(xr.readElement("baselevel", -1.0));
}

void SimpleExtrapolationFactoryImplementation::applyProxy()
{
    x->setBaseLevel(proxy->baseLevel());
}

void SimpleExtrapolationFactoryImplementation::resetProxy()
{
    proxy->setBaseLevel(x->baseLevel());
}

SimpleExtrapolationFactoryImplementation::~SimpleExtrapolationFactoryImplementation()
{
    XWriter xw("xdata/sml.xml");
    xw.writeElement("baselevel", x->baseLevel());

    delete proxy;
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

AbstractExtrapolationWidget *SimpleExtrapolationFactory::createProxyWidget() const
{
    m->resetProxy();

    SimpleExtrapolationWidget *widget = new SimpleExtrapolationWidget;
    widget->bindExtrapolator(m->proxy);

    return widget;
}

void SimpleExtrapolationFactory::applyProxyData()
{ m->applyProxy(); }

void SimpleExtrapolationFactory::resetProxyData()
{ m->resetProxy(); }

QString SimpleExtrapolationFactory::name() const
{ return SimpleExtrapolationWidget::tr("sml"); }

QString SimpleExtrapolationFactory::description() const
{ return SimpleExtrapolationWidget::tr("Simple"); }


SimpleExtrapolationFactory::~SimpleExtrapolationFactory()
{
    delete m;
}


} // namespace HeightMap
