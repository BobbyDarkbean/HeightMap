#include <cstdlib>
#include <ctime>
#include "mappingthread.h"


namespace HeightMap {


MappingThread::MappingThread(QObject *parent)
    : QThread(parent) { }


MappingThread::~MappingThread() { }


void MappingThread::run()
{
    using std::time;
    using std::srand;
    using std::rand;

    srand(static_cast<unsigned int>(time(nullptr)));
    volatile int r;
    r = rand();
    r = rand();
    (void)r;

    QThread::run();
}


} // namespace HeightMap
