#ifndef _ContouringOptionsWidget_h_
#define _ContouringOptionsWidget_h_


#include <QWidget>
#include "hm_shared.h"


namespace HeightMap {


struct ContouringOptionsWidgetImplementation;
class ContouringOptionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ContouringOptionsWidget(QWidget *parent = 0);

    int minLevel() const;
    void setMinLevel(int);

    int maxLevel() const;
    void setMaxLevel(int);

    int step() const;
    void setStep(int);

    ~ContouringOptionsWidget();

private:
    DISABLE_COPY(ContouringOptionsWidget)
    DISABLE_MOVE(ContouringOptionsWidget)

    ContouringOptionsWidgetImplementation *m;
};


} // namespace HeightMap


#endif // _ContouringOptionsWidget_h_
