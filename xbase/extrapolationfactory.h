#ifndef _ExtrapolationFactory_h_
#define _ExtrapolationFactory_h_


#include <QObject>
#include <QString>
#include "xbase_shared.h"
#include "hmdef.h"


namespace HeightMap {


class Extrapolator;
class AbstractExtrapolationWidget;
class ExtrapolationData;
class HMX_SHARED ExtrapolationFactory
{
public:
    ExtrapolationFactory();

    virtual Extrapolator *extrapolator() const = 0;

    virtual AbstractExtrapolationWidget *createWidget(bool bind) const = 0;

    virtual QString name() const = 0;
    virtual QString description() const = 0;

    virtual ExtrapolationData extractData() const = 0;
    virtual void provideData(const ExtrapolationData &) = 0;

    virtual ~ExtrapolationFactory() = 0;

private:
    DISABLE_COPY(ExtrapolationFactory)
    DISABLE_MOVE(ExtrapolationFactory)
};


} // namespace HeightMap


Q_DECLARE_INTERFACE(HeightMap::ExtrapolationFactory, "BobbyDarkbean.HeightMap.ExtrapolationInterface")


#endif // _ExtrapolationFactory_h_
