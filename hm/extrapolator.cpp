#include "extrapolator.h"


namespace HeightMap {


Extrapolator::Extrapolator()
    : m_baseLevel(0.0) { }


void Extrapolator::setBaseLevel(double l)
{ m_baseLevel = l; }


Extrapolator::~Extrapolator() { }


} // namespace HeightMap
