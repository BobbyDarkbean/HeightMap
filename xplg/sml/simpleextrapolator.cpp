#include "simpleextrapolator.h"


namespace HeightMap {


SimpleExtrapolator::SimpleExtrapolator()
    : Extrapolator() { }


double SimpleExtrapolator::radius(double peak) const
{ return peak; }

void SimpleExtrapolator::modify(
    double &currentValue,
    double peak,
    double vectorLength)
{
    double h = peak - vectorLength;         // extrapolated value
    double h_diff = peak - currentValue;    // current-peak difference
    double add_val = h_diff / peak * h;     // correction value

    currentValue += add_val;
}


SimpleExtrapolator::~SimpleExtrapolator() { }


} // namespace HeightMap
