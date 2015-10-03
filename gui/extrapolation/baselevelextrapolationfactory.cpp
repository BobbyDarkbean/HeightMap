#include "../widgets/baselevelextrapolationwidget.h"
#include "extrapolator.h"
#include "xreader.h"
#include "xwriter.h"

#include "baselevelextrapolationfactory.h"


namespace HeightMap {


struct BaseLevelExtrapolationFactoryImplementation
{
    BaseLevelExtrapolationFactoryImplementation();
    void applyProxy();
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
    x->setBaseLevel(xr.readElement("baselevel", -1.0));
}

void BaseLevelExtrapolationFactoryImplementation::applyProxy()
{
    x->setBaseLevel(proxy->baseLevel());
}

BaseLevelExtrapolationFactoryImplementation::~BaseLevelExtrapolationFactoryImplementation()
{
    XWriter xw("xdata/bsl.xml");
    xw.writeElement("baselevel", x->baseLevel());

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
    BaseLevelExtrapolationWidget *widget = new BaseLevelExtrapolationWidget;
    widget->bindExtrapolator(m->proxy);
    return widget;
}

void BaseLevelExtrapolationFactory::applyProxyData()
{ m->applyProxy(); }

QString BaseLevelExtrapolationFactory::name() const
{ return BaseLevelExtrapolationWidget::tr("bsl"); }

QString BaseLevelExtrapolationFactory::description() const
{ return BaseLevelExtrapolationWidget::tr("Base level depended"); }


BaseLevelExtrapolationFactory::~BaseLevelExtrapolationFactory()
{
    delete m;
}


} // namespace HeightMap
