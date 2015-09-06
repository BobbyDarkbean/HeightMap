#include "landscape.h"


namespace HeightMap {


struct LandscapeImplementation
{
    LandscapeImplementation(unsigned int width, unsigned int height);
    ~LandscapeImplementation();

    unsigned int w, h;
    double *data;

private:
    DISABLE_COPY(LandscapeImplementation)
};


LandscapeImplementation::LandscapeImplementation(unsigned int width, unsigned int height)
    : w(width),
      h(height),
      data(new double[w * h]) { }


LandscapeImplementation::~LandscapeImplementation()
{
    delete[] data;
}


Landscape::Landscape(unsigned int w, unsigned int h)
    : m(new LandscapeImplementation(w, h)) { }


unsigned int Landscape::width() const { return m->w; }
unsigned int Landscape::height() const { return m->h; }


double *Landscape::matrix() { return m->data; }
const double *Landscape::matrix() const { return m->data; }


} // namespace HeightMap
