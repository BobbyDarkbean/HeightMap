#ifndef _AbstractExtrapolationWidget_h_
#define _AbstractExtrapolationWidget_h_


#include <QWidget>
#include "hm_shared.h"


namespace HeightMap {


class Extrapolator;
class AbstractExtrapolationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AbstractExtrapolationWidget(QWidget *parent = 0);

    virtual Extrapolator *extrapolator() const = 0;
    virtual void refreshData() = 0;

    ~AbstractExtrapolationWidget();

private:
    DISABLE_COPY(AbstractExtrapolationWidget)
    DISABLE_MOVE(AbstractExtrapolationWidget)
};


} // namespace HeightMap


#endif // _AbstractExtrapolationWidget_h_
