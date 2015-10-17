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
    double h = peak - (peak / m_fixedRadius * vectorLength);    // extrapolated value
    double h_diff = peak - currentValue;                        // current-peak difference
    double add_val = h_diff / peak * h;                         // correction value

    currentValue += add_val;
}


FixedRadiusExtrapolator::~FixedRadiusExtrapolator() { }


} // namespace HeightMap
