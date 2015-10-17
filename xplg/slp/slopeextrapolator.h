#ifndef _SlopeExtrapolator_h_
#define _SlopeExtrapolator_h_


#include "extrapolator.h"


namespace HeightMap {


class SlopeExtrapolator : public Extrapolator
{
public:
    explicit SlopeExtrapolator(double slopeRatio = 1.0);

    double slopeRatio() const;
    void setSlopeRatio(double);

    double radius(double peak) const;
    void modify(double &currentValue,
                double peak,
                double vectorLength);

    ~SlopeExtrapolator();

private:
    double m_slopeRatio;
};


inline double SlopeExtrapolator::slopeRatio() const
{ return m_slopeRatio; }


} // namespace HeightMap


#endif // _SlopeExtrapolator_h_
