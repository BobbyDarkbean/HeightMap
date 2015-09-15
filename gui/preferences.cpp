#include <qglobal.h>
#include "preferences.h"


namespace HeightMap {


Preferences::Preferences()
    : m_lsWidth(DefaultLandscapeWidth),
      m_lsHeight(DefaultLandscapeHeight),
      m_peaks(DefaultPeakCount),
      m_minPeak(DefaultMinPeak),
      m_maxPeak(DefaultMaxPeak),
      m_lsBase(DefaultLandscapeBase),
      m_imgFactor(DefaultImageFactor) { }


void Preferences::setLandscapeWidth(int lsWidth)
{ m_lsWidth = qBound<int>(MinLandscapeDim, lsWidth, MaxLandscapeDim); }

void Preferences::setLandscapeHeight(int lsHeight)
{ m_lsHeight = qBound<int>(MinLandscapeDim, lsHeight, MaxLandscapeDim); }

void Preferences::setPeakCount(int peaks)
{ m_peaks = peaks; }

void Preferences::setMinPeak(int minPeak)
{ m_minPeak = qBound<int>(MinLevel, minPeak, MaxLevel); }

void Preferences::setMaxPeak(int maxPeak)
{ m_maxPeak = qBound<int>(MinLevel, maxPeak, MaxLevel); }

void Preferences::setLandscapeBase(int lsBase)
{ m_lsBase = qBound<int>(MinLevel, lsBase, MaxLevel); }

void Preferences::setImageFactor(int imgFactor)
{ m_imgFactor = qMax(1, imgFactor); }

bool Preferences::equals(const Preferences &other) const
{
    return (m_lsWidth == other.m_lsWidth)
        && (m_lsHeight == other.m_lsHeight)
        && (m_peaks == other.m_peaks)
        && (m_minPeak == other.m_minPeak)
        && (m_maxPeak == other.m_maxPeak)
        && (m_lsBase == other.m_lsBase)
        && (m_imgFactor == other.m_imgFactor);
}


} // namespace HeightMap
