#include "slopeextrapolationwidget.h"
#include "slopeextrapolator.h"
#include "extrapolationdata.h"
#include "xreader.h"
#include "xwriter.h"

#include "slopeextrapolationfactory.h"


namespace HeightMap {


struct SlopeExtrapolationFactoryImplementation
{
    SlopeExtrapolationFactoryImplementation();

    ExtrapolationData extract() const;
    void provide(const ExtrapolationData &);

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
    provide(xr.data());
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

    delete x;
}


SlopeExtrapolationFactory::SlopeExtrapolationFactory(QObject *parent)
    : QObject(parent),
      ExtrapolationFactory(),
      m(new SlopeExtrapolationFactoryImplementation) { }


Extrapolator *SlopeExtrapolationFactory::extrapolator() const
{ return m->x; }

AbstractExtrapolationWidget *SlopeExtrapolationFactory::createWidget(bool bind) const
{
    SlopeExtrapolationWidget *widget = new SlopeExtrapolationWidget;
    if (bind) {
        widget->bindExtrapolator(m->x);
    } else {
        widget->provideData(extractData());
    }

    return widget;
}

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
