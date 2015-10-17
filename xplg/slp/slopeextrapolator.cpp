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
    double h = peak - (vectorLength * m_slopeRatio);    // extrapolated value
    double h_diff = peak - currentValue;                // current-peak difference
    double add_val = h_diff / peak * h;                 // correction value

    currentValue += add_val;
}


SlopeExtrapolator::~SlopeExtrapolator() { }


} // namespace HeightMap
