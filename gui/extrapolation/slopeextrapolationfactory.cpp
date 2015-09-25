#include "../widgets/slopeextrapolationwidget.h"
#include "extrapolator.h"

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
    // read extrapolator settings from xdata/slope.xml
}

SlopeExtrapolationFactoryImplementation::~SlopeExtrapolationFactoryImplementation()
{
    // write extrapolator settings to xdata/slope.xml
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
