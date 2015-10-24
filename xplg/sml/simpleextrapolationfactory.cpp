#include <QApplication>
#include "simpleextrapolationwidget.h"
#include "simpleextrapolator.h"
#include "extrapolationdata.h"
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

    const QString XmlFilename;

private:
    DISABLE_COPY(SimpleExtrapolationFactoryImplementation)
    DISABLE_MOVE(SimpleExtrapolationFactoryImplementation)
};


SimpleExtrapolationFactoryImplementation::SimpleExtrapolationFactoryImplementation()
    : x(new SimpleExtrapolator),
      XmlFilename(qApp->applicationDirPath() + "/xdata/sml.xml")
{
    XReader xr(XmlFilename);
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
    XWriter xw(XmlFilename);
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
