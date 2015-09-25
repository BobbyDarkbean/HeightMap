#include "../widgets/baselevelextrapolationwidget.h"
#include "extrapolator.h"

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
    // read extrapolator settings from xdata/baselevel.xml
}

BaseLevelExtrapolationFactoryImplementation::~BaseLevelExtrapolationFactoryImplementation()
{
    // write extrapolator settings to xdata/baselevel.xml
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
