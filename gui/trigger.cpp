#include "trigger.h"


namespace HeightMap {


Trigger::Trigger(QObject *parent)
    : QObject(parent) { }


void Trigger::activate()
{
    emit activated();
}


Trigger::~Trigger() { }


} // namespace HeightMap
