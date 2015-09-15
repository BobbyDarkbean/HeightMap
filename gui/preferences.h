#ifndef _Preferences_h_
#define _Preferences_h_


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
        DefaultLandscapeBase    = 8,

        DefaultImageFactor      = 4
    };

    Preferences();

    int landscapeWidth() const;
    void setLandscapeWidth(int);

    int landscapeHeight() const;
    void setLandscapeHeight(int);

    int peakCount() const;
    void setPeakCount(int);

    int minPeak() const;
    void setMinPeak(int);

    int maxPeak() const;
    void setMaxPeak(int);

    int landscapeBase() const;
    void setLandscapeBase(int);

    int imageFactor() const;
    void setImageFactor(int);

    bool equals(const Preferences &other) const;

private:
    int m_lsWidth;
    int m_lsHeight;
    int m_peaks;
    int m_minPeak;
    int m_maxPeak;
    int m_lsBase;
    int m_imgFactor;
};


inline int Preferences::landscapeWidth() const
{ return m_lsWidth; }

inline int Preferences::landscapeHeight() const
{ return m_lsHeight; }

inline int Preferences::peakCount() const
{ return m_peaks; }

inline int Preferences::minPeak() const
{ return m_minPeak; }

inline int Preferences::maxPeak() const
{ return m_maxPeak; }

inline int Preferences::landscapeBase() const
{ return m_lsBase; }

inline int Preferences::imageFactor() const
{ return m_imgFactor; }


inline bool operator ==(const Preferences &a, const Preferences &b)
{ return a.equals(b); }

inline bool operator !=(const Preferences &a, const Preferences &b)
{ return !(a == b); }


} // namespace HeightMap


#endif // _Preferences_h_
