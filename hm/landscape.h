#ifndef _Landscape_h_
#define _Landscape_h_


#include "hm_shared.h"


namespace HeightMap {


struct LandscapeImplementation;
class HM_SHARED Landscape
{
public:
    Landscape(unsigned int w, unsigned int h);

    unsigned int width() const;
    unsigned int height() const;

    double *matrix();
    const double *matrix() const;

    ~Landscape();

private:
    DISABLE_COPY(Landscape)
    LandscapeImplementation *m;
};


} // namespace HeightMap


#endif // _Landscape_h_
