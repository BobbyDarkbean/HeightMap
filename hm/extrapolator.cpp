#include <cmath>
#include "extrapolator.h"


namespace HeightMap {


Extrapolator::Extrapolator()
    : m_baseLevel(0.0) { }


void Extrapolator::setBaseLevel(double l)
{ m_baseLevel = l; }


Extrapolator::~Extrapolator() { }


SimpleExtrapolator::SimpleExtrapolator()
    : Extrapolator() { }


int SimpleExtrapolator::radius(double peak) const
{ return static_cast<int>(peak); }

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


BaseLevelExtrapolator::BaseLevelExtrapolator()
    : Extrapolator() { }


int BaseLevelExtrapolator::radius(double peak) const
{ return static_cast<int>(std::abs(peak - baseLevel())); }

void BaseLevelExtrapolator::modify(
    double &currentValue,
    double peak,
    double vectorLength)
{
    double h = peak - (peak / radius(peak) * vectorLength);
    double h_diff = peak - currentValue;
    double add_val = h_diff / peak * h;

    currentValue += add_val;
}


BaseLevelExtrapolator::~BaseLevelExtrapolator() { }


SlopeExtrapolator::SlopeExtrapolator(double slopeRatio)
    : Extrapolator(),
      m_slopeRatio(slopeRatio) { }


void SlopeExtrapolator::setSlopeRatio(double r)
{ m_slopeRatio = r; }

int SlopeExtrapolator::radius(double peak) const
{ return static_cast<int>(peak / m_slopeRatio); }

void SlopeExtrapolator::modify(
    double &currentValue,
    double peak,
    double vectorLength)
{
    double h = peak - (vectorLength * m_slopeRatio);
    double h_diff = peak - currentValue;
    double add_val = h_diff / peak * h;

    currentValue += add_val;
}


SlopeExtrapolator::~SlopeExtrapolator() { }


FixedRadiusExtrapolator::FixedRadiusExtrapolator(int fixedRadius)
    : Extrapolator(),
      m_fixedRadius(fixedRadius) { }


void FixedRadiusExtrapolator::setFixedRadius(int r)
{ m_fixedRadius = r; }

int FixedRadiusExtrapolator::radius(double /* peak */) const
{ return static_cast<int>(m_fixedRadius); }

void FixedRadiusExtrapolator::modify(
    double &currentValue,
    double peak,
    double vectorLength)
{
    double h = peak - (peak / m_fixedRadius * vectorLength);
    double h_diff = peak - currentValue;
    double add_val = h_diff / peak * h;

    currentValue += add_val;
}


FixedRadiusExtrapolator::~FixedRadiusExtrapolator() { }


} // namespace HeightMap
