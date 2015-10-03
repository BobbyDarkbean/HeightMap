#include "../widgets/baselevelextrapolationwidget.h"
#include "extrapolator.h"
#include "xreader.h"
#include "xwriter.h"

#include "baselevelextrapolationfactory.h"


namespace HeightMap {


struct BaseLevelExtrapolationFactoryImplementation
{
    BaseLevelExtrapolationFactoryImplementation();
    ~BaseLevelExtrapolationFactoryImplementation();

    BaseLevelExtrapolator *x;

private:
    DISABLE_COPY(BaseLevelExtrapolationFactoryImplementation)
    DISABLE_MOVE(BaseLevelExtrapolationFactoryImplementation)
};


BaseLevelExtrapolationFactoryImplementation::BaseLevelExtrapolationFactoryImplementation()
    : x(new BaseLevelExtrapolator)
{
    XReader xr("xdata/bsl.xml");
    x->setBaseLevel(xr.readElement("baselevel", -1.0));
}

BaseLevelExtrapolationFactoryImplementation::~BaseLevelExtrapolationFactoryImplementation()
{
    XWriter xw("xdata/bsl.xml");
    xw.writeElement("baselevel", x->baseLevel());

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

QString BaseLevelExtrapolationFactory::name() const
{ return BaseLevelExtrapolationWidget::tr("bsl"); }

QString BaseLevelExtrapolationFactory::description() const
{ return BaseLevelExtrapolationWidget::tr("Base level depended"); }


BaseLevelExtrapolationFactory::~BaseLevelExtrapolationFactory()
{
    delete m;
}


} // namespace HeightMap
