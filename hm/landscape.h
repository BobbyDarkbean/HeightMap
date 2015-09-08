#ifndef _Landscape_h_
#define _Landscape_h_


#include <algorithm>
#include "hm_shared.h"


namespace HeightMap {


struct LandscapeImplementation;
class HM_SHARED Landscape
{
public:
    Landscape(int w, int h);
    Landscape(const Landscape &);

    int width() const;
    int height() const;

    double *matrix();
    const double *matrix() const;

    void swap(Landscape &);

    Landscape &operator=(Landscape);    // copy-and-swap
    ~Landscape();

private:
    LandscapeImplementation *m;
};


inline void swap(Landscape &a, Landscape &b)
{ a.swap(b); }


} // namespace HeightMap


namespace std {


template <>
inline void swap<HeightMap::Landscape>(
    HeightMap::Landscape &a,
    HeightMap::Landscape &b) { a.swap(b); }


} // namespace std


#endif // _Landscape_h_
