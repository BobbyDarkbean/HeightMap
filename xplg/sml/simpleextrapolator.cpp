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
    if (!(0.0 < peak))
        return;

    double h_diff = peak - currentValue;        // current-peak difference
    double d = h_diff / peak;                   // step
    double add_val = h_diff - d * vectorLength; // correction value

    currentValue += add_val;
}


SimpleExtrapolator::~SimpleExtrapolator() { }


} // namespace HeightMap
