#ifndef _FixedRadiusExtrapolationFactory_h_
#define _FixedRadiusExtrapolationFactory_h_


#include "extrapolationfactory.h"


namespace HeightMap {


struct FixedRadiusExtrapolationFactoryImplementation;
class FixedRadiusExtrapolationFactory : public ExtrapolationFactory
{
public:
    FixedRadiusExtrapolationFactory();

    Extrapolator *extrapolator() const;

    AbstractExtrapolationWidget *createWidget() const;
    AbstractExtrapolationWidget *createProxyWidget() const;

    void applyProxyData();

    QString name() const;
    QString description() const;

    ~FixedRadiusExtrapolationFactory();

private:
    DISABLE_COPY(FixedRadiusExtrapolationFactory)
    DISABLE_MOVE(FixedRadiusExtrapolationFactory)

    FixedRadiusExtrapolationFactoryImplementation *m;
};


} // namespace HeightMap


#endif // _FixedRadiusExtrapolationFactory_h_
