#include "simpleextrapolationwidget.h"
#include "extrapolationdata.h"
#include "extrapolator.h"
#include "xreader.h"
#include "xwriter.h"

#include "simpleextrapolationfactory.h"


namespace HeightMap {


struct SimpleExtrapolationFactoryImplementation
{
    SimpleExtrapolationFactoryImplementation();

    ExtrapolationData extract() const;
    void provide(const ExtrapolationData &);

    ~SimpleExtrapolationFactoryImplementation();

    SimpleExtrapolator *x;

private:
    DISABLE_COPY(SimpleExtrapolationFactoryImplementation)
    DISABLE_MOVE(SimpleExtrapolationFactoryImplementation)
};


SimpleExtrapolationFactoryImplementation::SimpleExtrapolationFactoryImplementation()
    : x(new SimpleExtrapolator)
{
    XReader xr("xdata/sml.xml");
    provide(xr.data());
}

ExtrapolationData SimpleExtrapolationFactoryImplementation::extract() const
{
    ExtrapolationData data;
    data.insert("baselevel", x->baseLevel());
    return data;
}

void SimpleExtrapolationFactoryImplementation::provide(const ExtrapolationData &data)
{
    x->setBaseLevel(data.value("baselevel", -1.0));
}

SimpleExtrapolationFactoryImplementation::~SimpleExtrapolationFactoryImplementation()
{
    XWriter xw("xdata/sml.xml");
    xw.setData(extract());

    delete x;
}


SimpleExtrapolationFactory::SimpleExtrapolationFactory(QObject *parent)
    : QObject(parent),
      ExtrapolationFactory(),
      m(new SimpleExtrapolationFactoryImplementation) { }


Extrapolator *SimpleExtrapolationFactory::extrapolator() const
{ return m->x; }

AbstractExtrapolationWidget *SimpleExtrapolationFactory::createWidget(bool bind) const
{
    SimpleExtrapolationWidget *widget = new SimpleExtrapolationWidget;
    if (bind) {
        widget->bindExtrapolator(m->x);
    } else {
        widget->provideData(extractData());
    }

    return widget;
}

QString SimpleExtrapolationFactory::name() const
{ return SimpleExtrapolationWidget::tr("sml"); }

QString SimpleExtrapolationFactory::description() const
{ return SimpleExtrapolationWidget::tr("Simple"); }

ExtrapolationData SimpleExtrapolationFactory::extractData() const
{ return m->extract(); }

void SimpleExtrapolationFactory::provideData(const ExtrapolationData &data)
{ m->provide(data); }


SimpleExtrapolationFactory::~SimpleExtrapolationFactory()
{
    delete m;
}


} // namespace HeightMap
