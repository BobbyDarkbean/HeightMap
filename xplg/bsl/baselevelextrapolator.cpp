#include <cmath>
#include "baselevelextrapolator.h"


namespace HeightMap {


BaseLevelExtrapolator::BaseLevelExtrapolator()
    : Extrapolator() { }


double BaseLevelExtrapolator::radius(double peak) const
{ return std::abs(peak - baseLevel()); }

void BaseLevelExtrapolator::modify(
    double &currentValue,
    double peak,
    double vectorLength)
{
    double h = peak - (peak / radius(peak) * vectorLength); // extrapolated value
    double h_diff = peak - currentValue;                    // current-peak difference
    double add_val = h_diff / peak * h;                     // correction value

    currentValue += add_val;
}


BaseLevelExtrapolator::~BaseLevelExtrapolator() { }


} // namespace HeightMap
