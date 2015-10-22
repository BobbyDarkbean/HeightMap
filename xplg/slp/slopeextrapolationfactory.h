#ifndef _SlopeExtrapolationFactory_h_
#define _SlopeExtrapolationFactory_h_


#include "extrapolationfactory.h"


namespace HeightMap {


struct SlopeExtrapolationFactoryImplementation;
class SlopeExtrapolationFactory
        : public QObject, public ExtrapolationFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "BobbyDarkbean.HeightMap.ExtrapolationInterface" FILE "slp.json")
    Q_INTERFACES(HeightMap::ExtrapolationFactory)

public:
    explicit SlopeExtrapolationFactory(QObject *parent = 0);

    Extrapolator *extrapolator() const;

    AbstractExtrapolationWidget *createWidget(bool bind) const;

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
