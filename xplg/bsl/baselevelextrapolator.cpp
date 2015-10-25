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
    double r = radius(peak);
    if (!(0.0 < r))
        return;

    double h_diff = peak - currentValue;        // current-peak difference
    double d = h_diff / r;                      // step
    double add_val = h_diff - d * vectorLength; // correction value

    currentValue += add_val;
}


BaseLevelExtrapolator::~BaseLevelExtrapolator() { }


} // namespace HeightMap
