#ifndef _SimpleExtrapolationFactory_h_
#define _SimpleExtrapolationFactory_h_


#include "extrapolationfactory.h"


namespace HeightMap {


struct SimpleExtrapolationFactoryImplementation;
class SimpleExtrapolationFactory
        : public QObject, public ExtrapolationFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "BobbyDarkbean.HeightMap.ExtrapolationInterface" FILE "sml.json")
    Q_INTERFACES(HeightMap::ExtrapolationFactory)

public:
    explicit SimpleExtrapolationFactory(QObject *parent = 0);

    Extrapolator *extrapolator() const;

    AbstractExtrapolationWidget *createWidget(bool bind) const;

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
