#ifndef _Trigger_h_
#define _Trigger_h_


#include <QObject>
#include "hmdef.h"


namespace HeightMap {


class Trigger : public QObject
{
    Q_OBJECT

public:
    explicit Trigger(QObject *parent = 0);
    virtual void activate();
    ~Trigger();

signals:
    void activated();

private:
    DISABLE_COPY(Trigger)
    DISABLE_MOVE(Trigger)
};


} // namespace HeightMap


#endif // _Trigger_h_
