#include "../widgets/slopeextrapolationwidget.h"
#include "extrapolator.h"
#include "xreader.h"
#include "xwriter.h"

#include "slopeextrapolationfactory.h"


namespace HeightMap {


struct SlopeExtrapolationFactoryImplementation
{
    SlopeExtrapolationFactoryImplementation();
    void applyProxy();
    ~SlopeExtrapolationFactoryImplementation();

    SlopeExtrapolator *x;
    SlopeExtrapolator *proxy;

private:
    DISABLE_COPY(SlopeExtrapolationFactoryImplementation)
    DISABLE_MOVE(SlopeExtrapolationFactoryImplementation)
};


SlopeExtrapolationFactoryImplementation::SlopeExtrapolationFactoryImplementation()
    : x(new SlopeExtrapolator),
      proxy(new SlopeExtrapolator)
{
    XReader xr("xdata/slp.xml");
    x->setBaseLevel(xr.readElement("baselevel", -1.0));
    x->setSlopeRatio(xr.readElement("ratio", -1.0));
}

void SlopeExtrapolationFactoryImplementation::applyProxy()
{
    x->setBaseLevel(proxy->baseLevel());
    x->setSlopeRatio(proxy->slopeRatio());
}

SlopeExtrapolationFactoryImplementation::~SlopeExtrapolationFactoryImplementation()
{
    XWriter xw("xdata/slp.xml");
    xw.writeElement("baselevel", x->baseLevel());
    xw.writeElement("ratio", x->slopeRatio());

    delete proxy;
    delete x;
}


SlopeExtrapolationFactory::SlopeExtrapolationFactory()
    : ExtrapolationFactory(),
      m(new SlopeExtrapolationFactoryImplementation) { }


Extrapolator *SlopeExtrapolationFactory::extrapolator() const
{ return m->x; }

AbstractExtrapolationWidget *SlopeExtrapolationFactory::createWidget() const
{
    SlopeExtrapolationWidget *widget = new SlopeExtrapolationWidget;
    widget->bindExtrapolator(m->x);
    return widget;
}

AbstractExtrapolationWidget *SlopeExtrapolationFactory::createProxyWidget() const
{
    SlopeExtrapolationWidget *widget = new SlopeExtrapolationWidget;
    widget->bindExtrapolator(m->proxy);
    return widget;
}

void SlopeExtrapolationFactory::applyProxyData()
{ m->applyProxy(); }

QString SlopeExtrapolationFactory::name() const
{ return SlopeExtrapolationWidget::tr("slp"); }

QString SlopeExtrapolationFactory::description() const
{ return SlopeExtrapolationWidget::tr("Slope depended"); }


SlopeExtrapolationFactory::~SlopeExtrapolationFactory()
{
    delete m;
}


} // namespace HeightMap
