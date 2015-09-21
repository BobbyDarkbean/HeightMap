#include "landscape.h"
#include "mapper.h"

#include "terrain.h"


namespace HeightMap {


struct TerrainImplementation
{
    TerrainImplementation(int width, int height);
    TerrainImplementation *clone() const;
    ~TerrainImplementation();

    Landscape ls;
    std::vector<PeakInfo> peaks;
    std::list<Line2dSegment> contours;

private:
    // private copy constructor for cloning
    TerrainImplementation(const TerrainImplementation &);
    // other copy-move methods are disabled
    TerrainImplementation &operator=(const TerrainImplementation &) = delete;
    DISABLE_MOVE(TerrainImplementation)
};


TerrainImplementation::TerrainImplementation(int width, int height)
    : ls(width, height),
      peaks(),
      contours() { }

TerrainImplementation *TerrainImplementation::clone() const
{ return new TerrainImplementation(*this); }

TerrainImplementation::~TerrainImplementation() { }

// private
TerrainImplementation::TerrainImplementation(const TerrainImplementation &other)
    : ls(other.ls),
      peaks(other.peaks),
      contours(other.contours) { }


Terrain::Terrain(int w, int h)
    : m(new TerrainImplementation(w, h)) { }

Terrain::Terrain(const Terrain &other)
    : m(other.m->clone()) { }

Terrain::Terrain(Terrain &&other)
    : m(new TerrainImplementation(1, 1)) { swap(other); }


int Terrain::width() const { return m->ls.width(); }
int Terrain::height() const { return m->ls.height(); }

const Landscape &Terrain::landscape() const { return m->ls; }

const std::vector<PeakInfo> &Terrain::peaks() const { return m->peaks; }
const std::list<Line2dSegment> &Terrain::contours() const { return m->contours; }


void Terrain::fillLandscape(
    double level)
{ m->ls.fill(level); }

void Terrain::generatePeaks(
    Mapper *mapper,
    const PeakGenerationOptions &opts)
{ mapper->generatePeaks(m->peaks, opts); }

void Terrain::extrapolatePeaks(
    Mapper *mapper)
{ mapper->extrapolatePeaks(m->ls, m->peaks); }

void Terrain::calculateContours(
    Mapper *mapper,
    const std::vector<int> &levelLayout)
{ mapper->calculateContours(m->ls, levelLayout, m->contours); }


void Terrain::swap(Terrain &other)
{
    using std::swap;
    swap(m, other.m);
}

Terrain &Terrain::operator=(Terrain other)
{
    swap(other);
    return *this;
}

Terrain::~Terrain()
{
    delete m;
}


} // namespace HeightMap
