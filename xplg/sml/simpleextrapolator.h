#ifndef _SimpleExtrapolator_h_
#define _SimpleExtrapolator_h_


#include "extrapolator.h"


namespace HeightMap {


class SimpleExtrapolator : public Extrapolator
{
public:
    SimpleExtrapolator();

    double radius(double peak) const;
    void modify(double &currentValue,
                double peak,
                double vectorLength);

    ~SimpleExtrapolator();
};


} // namespace HeightMap


#endif // _SimpleExtrapolator_h_
