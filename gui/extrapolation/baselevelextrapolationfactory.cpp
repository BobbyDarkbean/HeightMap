#include "../widgets/baselevelextrapolationwidget.h"
#include "extrapolationdata.h"
#include "extrapolator.h"
#include "xreader.h"
#include "xwriter.h"

#include "baselevelextrapolationfactory.h"


namespace HeightMap {


struct BaseLevelExtrapolationFactoryImplementation
{
    BaseLevelExtrapolationFactoryImplementation();

    void applyProxy();
    void resetProxy();

    ExtrapolationData extract() const;
    void provide(const ExtrapolationData &);

    ~BaseLevelExtrapolationFactoryImplementation();

    BaseLevelExtrapolator *x;
    BaseLevelExtrapolator *proxy;

private:
    DISABLE_COPY(BaseLevelExtrapolationFactoryImplementation)
    DISABLE_MOVE(BaseLevelExtrapolationFactoryImplementation)
};


BaseLevelExtrapolationFactoryImplementation::BaseLevelExtrapolationFactoryImplementation()
    : x(new BaseLevelExtrapolator),
      proxy(new BaseLevelExtrapolator)
{
    XReader xr("xdata/bsl.xml");
    provide(xr.data());
}

void BaseLevelExtrapolationFactoryImplementation::applyProxy()
{
    x->setBaseLevel(proxy->baseLevel());
}

void BaseLevelExtrapolationFactoryImplementation::resetProxy()
{
    proxy->setBaseLevel(x->baseLevel());
}

ExtrapolationData BaseLevelExtrapolationFactoryImplementation::extract() const
{
    ExtrapolationData data;
    data.insert("baselevel", x->baseLevel());
    return data;
}

void BaseLevelExtrapolationFactoryImplementation::provide(const ExtrapolationData &data)
{
    x->setBaseLevel(data.value("baselevel", -1.0));
}

BaseLevelExtrapolationFactoryImplementation::~BaseLevelExtrapolationFactoryImplementation()
{
    XWriter xw("xdata/bsl.xml");
    xw.setData(extract());

    delete proxy;
    delete x;
}


BaseLevelExtrapolationFactory::BaseLevelExtrapolationFactory()
    : ExtrapolationFactory(),
      m(new BaseLevelExtrapolationFactoryImplementation) { }


Extrapolator *BaseLevelExtrapolationFactory::extrapolator() const
{ return m->x; }

AbstractExtrapolationWidget *BaseLevelExtrapolationFactory::createWidget() const
{
    BaseLevelExtrapolationWidget *widget = new BaseLevelExtrapolationWidget;
    widget->bindExtrapolator(m->x);

    return widget;
}

AbstractExtrapolationWidget *BaseLevelExtrapolationFactory::createProxyWidget() const
{
    m->resetProxy();

    BaseLevelExtrapolationWidget *widget = new BaseLevelExtrapolationWidget;
    widget->bindExtrapolator(m->proxy);

    return widget;
}

void BaseLevelExtrapolationFactory::applyProxyData()
{ m->applyProxy(); }

void BaseLevelExtrapolationFactory::resetProxyData()
{ m->resetProxy(); }

QString BaseLevelExtrapolationFactory::name() const
{ return BaseLevelExtrapolationWidget::tr("bsl"); }

QString BaseLevelExtrapolationFactory::description() const
{ return BaseLevelExtrapolationWidget::tr("Base level depended"); }

ExtrapolationData BaseLevelExtrapolationFactory::extractData() const
{ return m->extract(); }

void BaseLevelExtrapolationFactory::provideData(const ExtrapolationData &data)
{ m->provide(data); }


BaseLevelExtrapolationFactory::~BaseLevelExtrapolationFactory()
{
    delete m;
}


} // namespace HeightMap
