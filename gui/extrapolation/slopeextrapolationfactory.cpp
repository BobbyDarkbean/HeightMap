#include "../widgets/slopeextrapolationwidget.h"
#include "extrapolator.h"
#include "xreader.h"
#include "xwriter.h"

#include "slopeextrapolationfactory.h"


namespace HeightMap {


struct SlopeExtrapolationFactoryImplementation
{
    SlopeExtrapolationFactoryImplementation();
    ~SlopeExtrapolationFactoryImplementation();

    SlopeExtrapolator *x;

private:
    DISABLE_COPY(SlopeExtrapolationFactoryImplementation)
    DISABLE_MOVE(SlopeExtrapolationFactoryImplementation)
};


SlopeExtrapolationFactoryImplementation::SlopeExtrapolationFactoryImplementation()
    : x(new SlopeExtrapolator)
{
    XReader xr("xdata/slp.xml");
    x->setBaseLevel(xr.readElement("baselevel", -1.0));
    x->setSlopeRatio(xr.readElement("ratio", -1.0));
}

SlopeExtrapolationFactoryImplementation::~SlopeExtrapolationFactoryImplementation()
{
    XWriter xw("xdata/slp.xml");
    xw.writeElement("baselevel", x->baseLevel());
    xw.writeElement("ratio", x->slopeRatio());

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

QString SlopeExtrapolationFactory::name() const
{ return SlopeExtrapolationWidget::tr("slp"); }

QString SlopeExtrapolationFactory::description() const
{ return SlopeExtrapolationWidget::tr("Slope depended"); }


SlopeExtrapolationFactory::~SlopeExtrapolationFactory()
{
    delete m;
}


} // namespace HeightMap
