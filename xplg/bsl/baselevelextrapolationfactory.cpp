#include <QApplication>
#include "baselevelextrapolationwidget.h"
#include "baselevelextrapolator.h"
#include "extrapolationdata.h"
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

    const QString XmlFilename;

private:
    DISABLE_COPY(BaseLevelExtrapolationFactoryImplementation)
    DISABLE_MOVE(BaseLevelExtrapolationFactoryImplementation)
};


BaseLevelExtrapolationFactoryImplementation::BaseLevelExtrapolationFactoryImplementation()
    : x(new BaseLevelExtrapolator),
      XmlFilename(qApp->applicationDirPath() + "/xdata/bsl.xml")
{
    XReader xr(XmlFilename);
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
    XWriter xw(XmlFilename);
    xw.setData(extract());

    delete x;
}


BaseLevelExtrapolationFactory::BaseLevelExtrapolationFactory(QObject *parent)
    : QObject(parent),
      ExtrapolationFactory(),
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
