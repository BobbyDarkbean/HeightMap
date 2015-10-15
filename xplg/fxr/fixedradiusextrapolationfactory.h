#ifndef _FixedRadiusExtrapolationFactory_h_
#define _FixedRadiusExtrapolationFactory_h_


#include "extrapolationfactory.h"


namespace HeightMap {


struct FixedRadiusExtrapolationFactoryImplementation;
class FixedRadiusExtrapolationFactory
        : public QObject, public ExtrapolationFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "BobbyDarkbean.HeightMap.ExtrapolationInterface" FILE "fxr.json")
    Q_INTERFACES(HeightMap::ExtrapolationFactory)

public:
    explicit FixedRadiusExtrapolationFactory(QObject *parent = 0);

    Extrapolator *extrapolator() const;

    AbstractExtrapolationWidget *createWidget(bool bind) const;

    QString name() const;
    QString description() const;

    ExtrapolationData extractData() const;
    void provideData(const ExtrapolationData &);

    ~FixedRadiusExtrapolationFactory();

private:
    DISABLE_COPY(FixedRadiusExtrapolationFactory)
    DISABLE_MOVE(FixedRadiusExtrapolationFactory)

    FixedRadiusExtrapolationFactoryImplementation *m;
};


} // namespace HeightMap


#endif // _FixedRadiusExtrapolationFactory_h_
