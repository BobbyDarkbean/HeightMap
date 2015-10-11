#ifndef _BaseLevelExtrapolationFactory_h_
#define _BaseLevelExtrapolationFactory_h_


#include "extrapolationfactory.h"


namespace HeightMap {


struct BaseLevelExtrapolationFactoryImplementation;
class BaseLevelExtrapolationFactory : public ExtrapolationFactory
{
public:
    BaseLevelExtrapolationFactory();

    Extrapolator *extrapolator() const;

    AbstractExtrapolationWidget *createWidget(bool bind) const;

    void applyProxyData();
    void resetProxyData();

    QString name() const;
    QString description() const;

    ExtrapolationData extractData() const;
    void provideData(const ExtrapolationData &);

    ~BaseLevelExtrapolationFactory();

private:
    DISABLE_COPY(BaseLevelExtrapolationFactory)
    DISABLE_MOVE(BaseLevelExtrapolationFactory)

    BaseLevelExtrapolationFactoryImplementation *m;
};


} // namespace HeightMap


#endif // _BaseLevelExtrapolationFactory_h_
