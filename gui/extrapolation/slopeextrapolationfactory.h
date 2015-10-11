#ifndef _SlopeExtrapolationFactory_h_
#define _SlopeExtrapolationFactory_h_


#include "extrapolationfactory.h"


namespace HeightMap {


struct SlopeExtrapolationFactoryImplementation;
class SlopeExtrapolationFactory : public ExtrapolationFactory
{
public:
    SlopeExtrapolationFactory();

    Extrapolator *extrapolator() const;

    AbstractExtrapolationWidget *createWidget(bool bind) const;

    void applyProxyData();
    void resetProxyData();

    QString name() const;
    QString description() const;

    ExtrapolationData extractData() const;
    void provideData(const ExtrapolationData &);

    ~SlopeExtrapolationFactory();

private:
    DISABLE_COPY(SlopeExtrapolationFactory)
    DISABLE_MOVE(SlopeExtrapolationFactory)

    SlopeExtrapolationFactoryImplementation *m;
};


} // namespace HeightMap


#endif // _SlopeExtrapolationFactory_h_
