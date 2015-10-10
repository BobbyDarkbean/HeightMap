#ifndef _Preferences_h_
#define _Preferences_h_


#include <algorithm>
#include <QString>


namespace HeightMap {


struct PreferencesImplementation;
class Preferences
{
public:

    enum {
        MinLandscapeDim         = 16,
        MaxLandscapeDim         = 2048,
        DefaultLandscapeWidth   = 256,
        DefaultLandscapeHeight  = 256,

        DefaultPeakCount        = 1024,

        MinLevel                = 0,
        MaxLevel                = 255,
        DefaultMinPeak          = 100,
        DefaultMaxPeak          = 200,

        DefaultMinContourLevel  = MinLevel + 1,
        DefaultMaxContourLevel  = MaxLevel,
        DefaultContouringStep   = 1,

        DefaultImageFactor      = 4
    };

    Preferences();
    Preferences(const Preferences &);
    Preferences(Preferences &&);

    int landscapeWidth() const;
    void setLandscapeWidth(int);

    int landscapeHeight() const;
    void setLandscapeHeight(int);

    unsigned int peakCount() const;
    void setPeakCount(unsigned int);

    int minPeak() const;
    void setMinPeak(int);

    int maxPeak() const;
    void setMaxPeak(int);

    const QString &extrapolatorName() const;
    void setExtrapolatorName(const QString &);

    int minContouringLevel() const;
    void setMinContouringLevel(int);

    int maxContouringLevel() const;
    void setMaxContouringLevel(int);

    int contouringStep() const;
    void setContouringStep(int);

    int imageFactor() const;
    void setImageFactor(int);

    bool equals(const Preferences &) const;

    void detach();
    void swap(Preferences &);

    Preferences &operator=(Preferences);    // copy-and-swap
    ~Preferences();

private:
    PreferencesImplementation *m;
};


inline bool operator ==(const Preferences &a, const Preferences &b)
{ return a.equals(b); }

inline bool operator !=(const Preferences &a, const Preferences &b)
{ return !(a == b); }


} // namespace HeightMap


namespace std {


template <>
inline void swap<HeightMap::Preferences>(
    HeightMap::Preferences &a,
    HeightMap::Preferences &b) { a.swap(b); }


} // namespace std


#endif // _Preferences_h_
