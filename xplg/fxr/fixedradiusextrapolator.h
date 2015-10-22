#ifndef _FixedRadiusExtrapolator_h_
#define _FixedRadiusExtrapolator_h_


#include "extrapolator.h"


namespace HeightMap {


class FixedRadiusExtrapolator : public Extrapolator
{
public:
    explicit FixedRadiusExtrapolator(int fixedRadius = 16);

    int fixedRadius() const;
    void setFixedRadius(int);

    double radius(double peak) const;
    void modify(double &currentValue,
                double peak,
                double vectorLength);

    ~FixedRadiusExtrapolator();

private:
    double m_fixedRadius;
};


inline int FixedRadiusExtrapolator::fixedRadius() const
{ return m_fixedRadius; }


} // namespace HeightMap


#endif // _FixedRadiusExtrapolator_h_
