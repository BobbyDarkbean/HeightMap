#ifndef _BaseLevelExtrapolator_h_
#define _BaseLevelExtrapolator_h_


#include "extrapolator.h"


namespace HeightMap {


class BaseLevelExtrapolator : public Extrapolator
{
public:
    BaseLevelExtrapolator();

    double radius(double peak) const;
    void modify(double &currentValue,
                double peak,
                double vectorLength);

    ~BaseLevelExtrapolator();
};


} // namespace HeightMap


#endif // _BaseLevelExtrapolator_h_
