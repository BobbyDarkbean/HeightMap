#ifndef _ContouringOptionsWidget_h_
#define _ContouringOptionsWidget_h_


#include <QWidget>
#include "hmdef.h"
#include "../ge_shared.h"


namespace HeightMap {


struct ContouringOptionsWidgetImplementation;
class HMGE_SHARED ContouringOptionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ContouringOptionsWidget(QWidget *parent = 0);

    bool eventFilter(QObject *, QEvent *);

    int minLevel() const;
    int maxLevel() const;
    void setLevelRange(int minValue, int maxValue);

    int step() const;
    void setStep(int);

    ~ContouringOptionsWidget();

signals:
    void minLevelChanged(int);
    void maxLevelChanged(int);
    void stepChanged(int);

private:
    DISABLE_COPY(ContouringOptionsWidget)
    DISABLE_MOVE(ContouringOptionsWidget)

    ContouringOptionsWidgetImplementation *m;
};


} // namespace HeightMap


#endif // _ContouringOptionsWidget_h_
