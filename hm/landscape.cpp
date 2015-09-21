#include <ostream>
#include "landscape.h"


namespace HeightMap {


struct LandscapeImplementation
{
    LandscapeImplementation(int width, int height);

    void fill(double level);
    LandscapeImplementation *clone() const;

    ~LandscapeImplementation();

    int w, h;
    double *const data;

private:
    // private copy constructor for cloning
    LandscapeImplementation(const LandscapeImplementation &);
    // other copy-move methods are disabled
    LandscapeImplementation &operator=(const LandscapeImplementation &) = delete;
    DISABLE_MOVE(LandscapeImplementation)
};


LandscapeImplementation::LandscapeImplementation(int width, int height)
    : w(width),
      h(height),
      data(new double[w * h]) { }

void LandscapeImplementation::fill(double level)
{
    double *itr = data;
    const double *const end = itr + (w * h);
    while (itr != end)
        *itr++ = level;
}

LandscapeImplementation *LandscapeImplementation::clone() const
{ return new LandscapeImplementation(*this); }

LandscapeImplementation::~LandscapeImplementation()
{
    delete[] data;
}

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


Landscape::Landscape(int w, int h)
    : m(new LandscapeImplementation(w, h)) { }

Landscape::Landscape(const Landscape &other)
    : m(other.m->clone()) { }

Landscape::Landscape(Landscape &&other)
    : m(new LandscapeImplementation(1, 1)) { swap(other); }


int Landscape::width() const { return m->w; }
int Landscape::height() const { return m->h; }

void Landscape::fill(double level) { m->fill(level); }

double *Landscape::matrix() { return m->data; }
const double *Landscape::matrix() const { return m->data; }

void Landscape::exportTo(
    std::ostream &stream,
    int precision) const
{
    stream.precision(precision);
    stream.flags(std::ios::fixed);

    // transposed matrix
    const double *const d = matrix();
    for (int i = 0; i < height(); ++i) {
        for (int j = 0; j < width(); ++j) {
            stream.width(12);
            stream << std::right << *(d + j * height() + i) /*<< '\t'*/;
        }
        stream << '\n';
    }
}


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
