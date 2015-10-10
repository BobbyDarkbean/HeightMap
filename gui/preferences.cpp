#include <qglobal.h>
#include "hm_shared.h"

#include "preferences.h"


namespace HeightMap {


struct PreferencesImplementation
{
    PreferencesImplementation();

    bool content_equals(const PreferencesImplementation &);
    PreferencesImplementation *clone() const;

    ~PreferencesImplementation();

    int lsWidth;
    int lsHeight;
    unsigned int peaks;
    int minPeak;
    int maxPeak;
    QString extrapolName;
    int minContLevel;
    int maxContLevel;
    int contStep;
    int imgFactor;

    int ref;

private:
    // private copy constructor for cloning
    PreferencesImplementation(const PreferencesImplementation &);
    // other copy-move methods are disabled
    PreferencesImplementation &operator=(const PreferencesImplementation &) = delete;
    DISABLE_MOVE(PreferencesImplementation)
};


PreferencesImplementation::PreferencesImplementation()
    : lsWidth(Preferences::DefaultLandscapeWidth),
      lsHeight(Preferences::DefaultLandscapeHeight),
      peaks(Preferences::DefaultPeakCount),
      minPeak(Preferences::DefaultMinPeak),
      maxPeak(Preferences::DefaultMaxPeak),
      extrapolName(),
      minContLevel(Preferences::DefaultMinContourLevel),
      maxContLevel(Preferences::DefaultMaxContourLevel),
      contStep(Preferences::DefaultContouringStep),
      imgFactor(Preferences::DefaultImageFactor),
      ref(1) { }

bool PreferencesImplementation::content_equals(const PreferencesImplementation &other)
{
    return (lsWidth == other.lsWidth)
        && (lsHeight == other.lsHeight)
        && (peaks == other.peaks)
        && (minPeak == other.minPeak)
        && (maxPeak == other.maxPeak)
        && (extrapolName == other.extrapolName)
        && (minContLevel == other.minContLevel)
        && (maxContLevel == other.maxContLevel)
        && (contStep == other.contStep)
        && (imgFactor == other.imgFactor);
}

PreferencesImplementation *PreferencesImplementation::clone() const
{ return new PreferencesImplementation(*this); }

PreferencesImplementation::~PreferencesImplementation() { }

// private
PreferencesImplementation::PreferencesImplementation(const PreferencesImplementation &other)
    : lsWidth(other.lsWidth),
      lsHeight(other.lsHeight),
      peaks(other.peaks),
      minPeak(other.minPeak),
      maxPeak(other.maxPeak),
      extrapolName(other.extrapolName),
      minContLevel(other.minContLevel),
      maxContLevel(other.maxContLevel),
      contStep(other.contStep),
      imgFactor(other.imgFactor),
      ref(1) { }


Preferences::Preferences()
    : m(new PreferencesImplementation) { }

Preferences::Preferences(const Preferences &other)
    : m(other.m)
{
    ++m->ref;
}

Preferences::Preferences(Preferences &&other)
    : m(other.m)
{
    other.m = nullptr;
}


int Preferences::landscapeWidth() const
{ return m->lsWidth; }

void Preferences::setLandscapeWidth(int lsWidth)
{ m->lsWidth = qBound<int>(MinLandscapeDim, lsWidth, MaxLandscapeDim); }

int Preferences::landscapeHeight() const
{ return m->lsHeight; }

void Preferences::setLandscapeHeight(int lsHeight)
{ m->lsHeight = qBound<int>(MinLandscapeDim, lsHeight, MaxLandscapeDim); }

unsigned int Preferences::peakCount() const
{ return m->peaks; }

void Preferences::setPeakCount(unsigned int peaks)
{ m->peaks = peaks; }

int Preferences::minPeak() const
{ return m->minPeak; }

void Preferences::setMinPeak(int minPeak)
{ m->minPeak = qBound<int>(MinLevel, minPeak, MaxLevel); }

int Preferences::maxPeak() const
{ return m->maxPeak; }

void Preferences::setMaxPeak(int maxPeak)
{ m->maxPeak = qBound<int>(MinLevel, maxPeak, MaxLevel); }

const QString &Preferences::extrapolatorName() const
{ return m->extrapolName; }

void Preferences::setExtrapolatorName(const QString &extrapolName)
{ m->extrapolName = extrapolName; }

int Preferences::minContouringLevel() const
{ return m->minContLevel; }

void Preferences::setMinContouringLevel(int minContLevel)
{ m->minContLevel = qBound<int>(1, minContLevel, MaxLevel); }

int Preferences::maxContouringLevel() const
{ return m->maxContLevel; }

void Preferences::setMaxContouringLevel(int maxContLevel)
{ m->maxContLevel = qBound<int>(1, maxContLevel, MaxLevel); }

int Preferences::contouringStep() const
{ return m->contStep; }

void Preferences::setContouringStep(int contStep)
{ m->contStep = qBound<int>(1, contStep, MaxLevel); }

int Preferences::imageFactor() const
{ return m->imgFactor; }

void Preferences::setImageFactor(int imgFactor)
{ m->imgFactor = qMax(1, imgFactor); }


bool Preferences::equals(const Preferences &other) const
{ return m == other.m ? true : m->content_equals(*other.m); }

void Preferences::detach()
{
    if (m->ref > 1) {
        --m->ref;
        m = m->clone();
    }
}

void Preferences::swap(Preferences &other)
{
    using std::swap;
    swap(m, other.m);
}

Preferences &Preferences::operator=(Preferences other)
{
    swap(other);
    return *this;
}

Preferences::~Preferences()
{
    if (m) {
        if (--m->ref == 0)
            delete m;
    }
}


} // namespace HeightMap
