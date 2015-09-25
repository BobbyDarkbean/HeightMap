#ifndef _Terrain_h_
#define _Terrain_h_


#include <vector>
#include <list>
#include <algorithm>
#include "hm_shared.h"
#include "peakinfo.h"
#include "line2dsegment.h"


namespace HeightMap {


struct PeakGenerationOptions;
class Landscape;
class Mapper;
class Extrapolator;
struct TerrainImplementation;
class HM_SHARED Terrain
{
public:
    Terrain(int w, int h);
    Terrain(const Terrain &);
    Terrain(Terrain &&);

    int width() const;
    int height() const;

    const Landscape &landscape() const;
    const std::vector<PeakInfo> &peaks() const;
    const std::list<Line2dSegment> &contours() const;

    void clearLandscape();
    void clearPeaks();
    void clearContours();

    void fillLandscape(double level);

    void generatePeaks(Mapper *mapper,
                       const PeakGenerationOptions &opts);
    void extrapolatePeaks(Mapper *mapper,
                          Extrapolator *extrapolator);
    void calculateContours(Mapper *mapper,
                           const std::vector<int> &levelLayout);

    void exportPeaks(std::ostream &) const;
    void exportLandscape(std::ostream &,
                         int precision) const;

    void swap(Terrain &);

    Terrain &operator=(Terrain);    // copy-and-swap
    ~Terrain();

private:
    TerrainImplementation *m;
};


} // namespace HeightMap


namespace std {


template <>
inline void swap<HeightMap::Terrain>(
    HeightMap::Terrain &a,
    HeightMap::Terrain &b) { a.swap(b); }


} // namespace std


#endif // _Terrain_h_
