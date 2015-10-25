#include "fixedradiusextrapolator.h"


namespace HeightMap {


FixedRadiusExtrapolator::FixedRadiusExtrapolator(int fixedRadius)
    : Extrapolator(),
      m_fixedRadius(fixedRadius) { }


void FixedRadiusExtrapolator::setFixedRadius(int r)
{ m_fixedRadius = r; }

double FixedRadiusExtrapolator::radius(double /* peak */) const
{ return m_fixedRadius; }

void FixedRadiusExtrapolator::modify(
    double &currentValue,
    double peak,
    double vectorLength)
{
    double h_diff = peak - currentValue;        // current-peak difference
    double d = h_diff / m_fixedRadius;          // step
    double add_val = h_diff - d * vectorLength; // correction value

    currentValue += add_val;
}


FixedRadiusExtrapolator::~FixedRadiusExtrapolator() { }


} // namespace HeightMap
