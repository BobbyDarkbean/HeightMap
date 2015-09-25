#include "../widgets/fixedradiusextrapolationwidget.h"
#include "extrapolator.h"

#include "fixedradiusextrapolationfactory.h"


namespace HeightMap {


struct FixedRadiusExtrapolationFactoryImplementation
{
    FixedRadiusExtrapolationFactoryImplementation();
    ~FixedRadiusExtrapolationFactoryImplementation();

    FixedRadiusExtrapolator *x;

private:
    DISABLE_COPY(FixedRadiusExtrapolationFactoryImplementation)
    DISABLE_MOVE(FixedRadiusExtrapolationFactoryImplementation)
};


FixedRadiusExtrapolationFactoryImplementation::FixedRadiusExtrapolationFactoryImplementation()
    : x(new FixedRadiusExtrapolator)
{
    // read extrapolator settings from xdata/fixedradius.xml
}

FixedRadiusExtrapolationFactoryImplementation::~FixedRadiusExtrapolationFactoryImplementation()
{
    // write extrapolator settings to xdata/fixedradius.xml
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

QString FixedRadiusExtrapolationFactory::name() const
{ return FixedRadiusExtrapolationWidget::tr("fxr"); }

QString FixedRadiusExtrapolationFactory::description() const
{ return FixedRadiusExtrapolationWidget::tr("Fixed radius"); }


FixedRadiusExtrapolationFactory::~FixedRadiusExtrapolationFactory()
{
    delete m;
}


} // namespace HeightMap
