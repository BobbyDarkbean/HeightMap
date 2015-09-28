#ifndef _Preferences_h_
#define _Preferences_h_


#include <QString>


namespace HeightMap {


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

    bool equals(const Preferences &other) const;

private:
    int m_lsWidth;
    int m_lsHeight;
    unsigned int m_peaks;
    int m_minPeak;
    int m_maxPeak;
    QString m_extrapolName;
    int m_minContLevel;
    int m_maxContLevel;
    int m_contStep;
    int m_imgFactor;
};


inline int Preferences::landscapeWidth() const
{ return m_lsWidth; }

inline int Preferences::landscapeHeight() const
{ return m_lsHeight; }

inline unsigned int Preferences::peakCount() const
{ return m_peaks; }

inline int Preferences::minPeak() const
{ return m_minPeak; }

inline int Preferences::maxPeak() const
{ return m_maxPeak; }

inline const QString &Preferences::extrapolatorName() const
{ return m_extrapolName; }

inline int Preferences::minContouringLevel() const
{ return m_minContLevel; }

inline int Preferences::maxContouringLevel() const
{ return m_maxContLevel; }

inline int Preferences::contouringStep() const
{ return m_contStep; }

inline int Preferences::imageFactor() const
{ return m_imgFactor; }


inline bool operator ==(const Preferences &a, const Preferences &b)
{ return a.equals(b); }

inline bool operator !=(const Preferences &a, const Preferences &b)
{ return !(a == b); }


} // namespace HeightMap


#endif // _Preferences_h_
