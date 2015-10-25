#include "slopeextrapolator.h"


namespace HeightMap {


SlopeExtrapolator::SlopeExtrapolator(double slopeRatio)
    : Extrapolator(),
      m_slopeRatio(slopeRatio) { }


void SlopeExtrapolator::setSlopeRatio(double r)
{ m_slopeRatio = r; }

double SlopeExtrapolator::radius(double peak) const
{ return peak / m_slopeRatio; }

void SlopeExtrapolator::modify(
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


SlopeExtrapolator::~SlopeExtrapolator() { }


} // namespace HeightMap
