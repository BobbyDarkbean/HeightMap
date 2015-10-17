#ifndef _BaseLevelExtrapolationFactory_h_
#define _BaseLevelExtrapolationFactory_h_


#include "extrapolationfactory.h"


namespace HeightMap {


struct BaseLevelExtrapolationFactoryImplementation;
class BaseLevelExtrapolationFactory
        : public QObject, public ExtrapolationFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "BobbyDarkbean.HeightMap.ExtrapolationInterface" FILE "bsl.json")
    Q_INTERFACES(HeightMap::ExtrapolationFactory)

public:
    explicit BaseLevelExtrapolationFactory(QObject *parent = 0);

    Extrapolator *extrapolator() const;

    AbstractExtrapolationWidget *createWidget(bool bind) const;

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
