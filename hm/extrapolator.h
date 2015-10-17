#ifndef _Extrapolator_h_
#define _Extrapolator_h_


#include "hm_shared.h"


namespace HeightMap {


class HM_SHARED Extrapolator
{
public:
    Extrapolator();

    double baseLevel() const;
    void setBaseLevel(double);

    virtual double radius(double peak) const = 0;
    virtual void modify(double &currentValue,
                        double peak,
                        double vectorLength) = 0;

    virtual ~Extrapolator() = 0;

private:
    double m_baseLevel;
};


inline double Extrapolator::baseLevel() const
{ return m_baseLevel; }


} // namespace HeightMap


#endif // _Extrapolator_h_
