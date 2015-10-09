#ifndef _SimpleExtrapolationFactory_h_
#define _SimpleExtrapolationFactory_h_


#include "extrapolationfactory.h"


namespace HeightMap {


struct SimpleExtrapolationFactoryImplementation;
class SimpleExtrapolationFactory : public ExtrapolationFactory
{
public:
    SimpleExtrapolationFactory();

    Extrapolator *extrapolator() const;

    AbstractExtrapolationWidget *createWidget() const;
    AbstractExtrapolationWidget *createProxyWidget() const;

    void applyProxyData();
    void resetProxyData();

    QString name() const;
    QString description() const;

    ExtrapolationData extractData() const;
    void provideData(const ExtrapolationData &);

    ~SimpleExtrapolationFactory();

private:
    DISABLE_COPY(SimpleExtrapolationFactory)
    DISABLE_MOVE(SimpleExtrapolationFactory)

    SimpleExtrapolationFactoryImplementation *m;
};


} // namespace HeightMap


#endif // _SimpleExtrapolationFactory_h_
