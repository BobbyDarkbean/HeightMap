#include "../widgets/slopeextrapolationwidget.h"
#include "extrapolationdata.h"
#include "extrapolator.h"
#include "xreader.h"
#include "xwriter.h"

#include "slopeextrapolationfactory.h"


namespace HeightMap {


struct SlopeExtrapolationFactoryImplementation
{
    SlopeExtrapolationFactoryImplementation();

    void applyProxy();
    void resetProxy();

    ExtrapolationData extract() const;
    void provide(const ExtrapolationData &);

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
    provide(xr.data());
}

void SlopeExtrapolationFactoryImplementation::applyProxy()
{
    x->setBaseLevel(proxy->baseLevel());
    x->setSlopeRatio(proxy->slopeRatio());
}

void SlopeExtrapolationFactoryImplementation::resetProxy()
{
    proxy->setBaseLevel(x->baseLevel());
    proxy->setSlopeRatio(x->slopeRatio());
}

ExtrapolationData SlopeExtrapolationFactoryImplementation::extract() const
{
    ExtrapolationData data;
    data.insert("baselevel", x->baseLevel());
    data.insert("ratio", x->slopeRatio());
    return data;
}

void SlopeExtrapolationFactoryImplementation::provide(const ExtrapolationData &data)
{
    x->setBaseLevel(data.value("baselevel", -1.0));
    x->setSlopeRatio(data.value("ratio", -1.0));
}

SlopeExtrapolationFactoryImplementation::~SlopeExtrapolationFactoryImplementation()
{
    XWriter xw("xdata/slp.xml");
    xw.setData(extract());

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
    m->resetProxy();

    SlopeExtrapolationWidget *widget = new SlopeExtrapolationWidget;
    widget->bindExtrapolator(m->proxy);

    return widget;
}

void SlopeExtrapolationFactory::applyProxyData()
{ m->applyProxy(); }

void SlopeExtrapolationFactory::resetProxyData()
{ m->resetProxy(); }

QString SlopeExtrapolationFactory::name() const
{ return SlopeExtrapolationWidget::tr("slp"); }

QString SlopeExtrapolationFactory::description() const
{ return SlopeExtrapolationWidget::tr("Slope depended"); }

ExtrapolationData SlopeExtrapolationFactory::extractData() const
{ return m->extract(); }

void SlopeExtrapolationFactory::provideData(const ExtrapolationData &data)
{ m->provide(data); }


SlopeExtrapolationFactory::~SlopeExtrapolationFactory()
{
    delete m;
}


} // namespace HeightMap
