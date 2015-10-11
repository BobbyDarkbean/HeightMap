#ifndef _ExtrapolationFactory_h_
#define _ExtrapolationFactory_h_


#include <QString>
#include "hm_shared.h"


namespace HeightMap {


class Extrapolator;
class AbstractExtrapolationWidget;
class ExtrapolationData;
class ExtrapolationFactory
{
public:
    ExtrapolationFactory();

    virtual Extrapolator *extrapolator() const = 0;

    virtual AbstractExtrapolationWidget *createWidget(bool bind) const = 0;

    virtual void applyProxyData() = 0;
    virtual void resetProxyData() = 0;

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


#endif // _ExtrapolationFactory_h_
