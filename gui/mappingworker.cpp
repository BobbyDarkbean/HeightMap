#include "mapper.h"
#include "mappingworker.h"


namespace HeightMap {


MappingWorker::MappingWorker(QObject *parent)
    : QObject(parent)
{
}


MappingWorker::~MappingWorker()
{
}


void MappingWorker::generatePeaks()
{
}

void MappingWorker::extrapolatePeaks()
{
}

void MappingWorker::calculateContours()
{
}


} // namespace HeightMap
