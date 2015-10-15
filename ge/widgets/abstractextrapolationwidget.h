#ifndef _AbstractExtrapolationWidget_h_
#define _AbstractExtrapolationWidget_h_


#include <QWidget>
#include "hmdef.h"
#include "../ge_shared.h"


namespace HeightMap {


class Extrapolator;
class ExtrapolationData;
class HMGE_SHARED AbstractExtrapolationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AbstractExtrapolationWidget(QWidget *parent = 0);

    virtual Extrapolator *extrapolator() const = 0;
    virtual void refreshData() = 0;

    virtual ExtrapolationData extractData() const = 0;
    virtual void provideData(const ExtrapolationData &) = 0;

    ~AbstractExtrapolationWidget();

private:
    DISABLE_COPY(AbstractExtrapolationWidget)
    DISABLE_MOVE(AbstractExtrapolationWidget)
};


} // namespace HeightMap


#endif // _AbstractExtrapolationWidget_h_
