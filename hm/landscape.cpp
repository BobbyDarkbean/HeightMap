#include "landscape.h"


namespace HeightMap {


struct LandscapeImplementation
{
    LandscapeImplementation(unsigned int width, unsigned int height);
    LandscapeImplementation *clone() const;
    ~LandscapeImplementation();

    unsigned int w, h;
    double *const data;

private:
    // private copy constructor for cloning
    LandscapeImplementation(const LandscapeImplementation &);
    // assignment operator is disabled
    LandscapeImplementation &operator=(const LandscapeImplementation &);
};


LandscapeImplementation::LandscapeImplementation(unsigned int width, unsigned int height)
    : w(width),
      h(height),
      data(new double[w * h]) { }

// private
LandscapeImplementation::LandscapeImplementation(const LandscapeImplementation &other)
    : w(other.w),
      h(other.h),
      data(new double[w * h])
{
    const double *from = other.data;
    const double *const end = from + (w * h);
    double *to = data;
    while (from != end)
        *to++ = *from++;
}

LandscapeImplementation *LandscapeImplementation::clone() const
{ return new LandscapeImplementation(*this); }

LandscapeImplementation::~LandscapeImplementation()
{
    delete[] data;
}


Landscape::Landscape(unsigned int w, unsigned int h)
    : m(new LandscapeImplementation(w, h)) { }

Landscape::Landscape(const Landscape &other)
    : m(other.m->clone()) { }


unsigned int Landscape::width() const { return m->w; }
unsigned int Landscape::height() const { return m->h; }

double *Landscape::matrix() { return m->data; }
const double *Landscape::matrix() const { return m->data; }


void Landscape::swap(Landscape &other)
{
    using std::swap;
    swap(m, other.m);
}

Landscape &Landscape::operator=(Landscape other)
{
    swap(other);
    return *this;
}

Landscape::~Landscape()
{
    delete m;
}


} // namespace HeightMap
