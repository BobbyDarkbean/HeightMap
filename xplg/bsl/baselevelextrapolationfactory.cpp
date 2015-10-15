#include "baselevelextrapolationwidget.h"
#include "extrapolationdata.h"
#include "extrapolator.h"
#include "xreader.h"
#include "xwriter.h"

#include "baselevelextrapolationfactory.h"


namespace HeightMap {


struct BaseLevelExtrapolationFactoryImplementation
{
    BaseLevelExtrapolationFactoryImplementation();

    ExtrapolationData extract() const;
    void provide(const ExtrapolationData &);

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
    provide(xr.data());
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

    delete x;
}


BaseLevelExtrapolationFactory::BaseLevelExtrapolationFactory()
    : ExtrapolationFactory(),
      m(new BaseLevelExtrapolationFactoryImplementation) { }


Extrapolator *BaseLevelExtrapolationFactory::extrapolator() const
{ return m->x; }

AbstractExtrapolationWidget *BaseLevelExtrapolationFactory::createWidget(bool bind) const
{
    BaseLevelExtrapolationWidget *widget = new BaseLevelExtrapolationWidget;
    if (bind) {
        widget->bindExtrapolator(m->x);
    } else {
        widget->provideData(extractData());
    }

    return widget;
}

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
