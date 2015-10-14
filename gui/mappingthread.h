#ifndef _MappingThread_h_
#define _MappingThread_h_


#include <QThread>
#include "hmdef.h"


namespace HeightMap {


class MappingThread : public QThread
{
    Q_OBJECT

public:
    explicit MappingThread(QObject *parent = 0);
    ~MappingThread();

protected:
    void run();

private:
    DISABLE_COPY(MappingThread)
    DISABLE_MOVE(MappingThread)
};


} // namespace HeightMap


#endif // _MappingThread_h_
