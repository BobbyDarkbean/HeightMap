#include "../widgets/fixedradiusextrapolationwidget.h"
#include "extrapolator.h"
#include "xreader.h"
#include "xwriter.h"

#include "fixedradiusextrapolationfactory.h"


namespace HeightMap {


struct FixedRadiusExtrapolationFactoryImplementation
{
    FixedRadiusExtrapolationFactoryImplementation();

    void applyProxy();
    void resetProxy();

    ~FixedRadiusExtrapolationFactoryImplementation();

    FixedRadiusExtrapolator *x;
    FixedRadiusExtrapolator *proxy;

private:
    DISABLE_COPY(FixedRadiusExtrapolationFactoryImplementation)
    DISABLE_MOVE(FixedRadiusExtrapolationFactoryImplementation)
};


FixedRadiusExtrapolationFactoryImplementation::FixedRadiusExtrapolationFactoryImplementation()
    : x(new FixedRadiusExtrapolator),
      proxy(new FixedRadiusExtrapolator)
{
    XReader xr("xdata/fxr.xml");
    x->setBaseLevel(xr.readElement("baselevel", -1.0));
    x->setFixedRadius(xr.readElement("radius", -1.0));
}

void FixedRadiusExtrapolationFactoryImplementation::applyProxy()
{
    x->setBaseLevel(proxy->baseLevel());
    x->setFixedRadius(proxy->fixedRadius());
}

void FixedRadiusExtrapolationFactoryImplementation::resetProxy()
{
    proxy->setBaseLevel(x->baseLevel());
    proxy->setFixedRadius(x->fixedRadius());
}

FixedRadiusExtrapolationFactoryImplementation::~FixedRadiusExtrapolationFactoryImplementation()
{
    XWriter xw("xdata/fxr.xml");
    xw.writeElement("baselevel", x->baseLevel());
    xw.writeElement("radius", x->fixedRadius());

    delete proxy;
    delete x;
}


FixedRadiusExtrapolationFactory::FixedRadiusExtrapolationFactory()
    : ExtrapolationFactory(),
      m(new FixedRadiusExtrapolationFactoryImplementation) { }


Extrapolator *FixedRadiusExtrapolationFactory::extrapolator() const
{ return m->x; }

AbstractExtrapolationWidget *FixedRadiusExtrapolationFactory::createWidget() const
{
    FixedRadiusExtrapolationWidget *widget = new FixedRadiusExtrapolationWidget;
    widget->bindExtrapolator(m->x);

    return widget;
}

AbstractExtrapolationWidget *FixedRadiusExtrapolationFactory::createProxyWidget() const
{
    m->resetProxy();

    FixedRadiusExtrapolationWidget *widget = new FixedRadiusExtrapolationWidget;
    widget->bindExtrapolator(m->proxy);

    return widget;
}

void FixedRadiusExtrapolationFactory::applyProxyData()
{ m->applyProxy(); }

void FixedRadiusExtrapolationFactory::resetProxyData()
{ m->resetProxy(); }

QString FixedRadiusExtrapolationFactory::name() const
{ return FixedRadiusExtrapolationWidget::tr("fxr"); }

QString FixedRadiusExtrapolationFactory::description() const
{ return FixedRadiusExtrapolationWidget::tr("Fixed radius"); }


FixedRadiusExtrapolationFactory::~FixedRadiusExtrapolationFactory()
{
    delete m;
}


} // namespace HeightMap
