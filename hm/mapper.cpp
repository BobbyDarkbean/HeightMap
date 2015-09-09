#include <cstdlib>
#include <ctime>
#include <cmath>
#include "internal/contouring.h"
#include "landscape.h"

#include "mapper.h"


namespace HeightMap {


namespace {
bool &rand_flag()
{
    static bool b = false;
    return b;
}
}


Mapper::Mapper()
    : init_event(peakGenerated, PeakInfo),
      init_event(peakExtrapolated, PeakInfo),
      init_event(contouringAt, PeakInfo)
{
    if (!rand_flag()) {
        srand(static_cast<unsigned int>(time(0)));
        volatile int r;
        r = rand();
        r = rand();
        (void)r;
        rand_flag() = true;
    }
}


void Mapper::generatePeaks(
    std::vector<PeakInfo> &peaks,
    PeakGenerationOptions opts)
{
    // swapping peak bounds if needed
    if (opts.maxPeak < opts.minPeak) {
        std::swap(opts.maxPeak, opts.minPeak);
    }
    int range = opts.maxPeak - opts.minPeak + 1;

    peaks.resize(opts.peakCount);

    // generating peaks
    for (unsigned int i = 0; i < opts.peakCount; ++i) {
        PeakInfo peak = {
            rand() % opts.hmWidth,
            rand() % opts.hmHeight,
            static_cast<double>(rand() % range + opts.minPeak)
        };

        peaks[i] = peak;
        raise_event(peakGenerated, PeakInfo, peak);
    }
}


void Mapper::extrapolatePeaks(
    Landscape &ls,
    const std::vector<PeakInfo> &peaks)
{
    /* Radial extrapolation */

    using std::min;
    using std::max;
    using std::sqrt;

    double *hm = ls.matrix();
    int hm_width = ls.width();
    int hm_height = ls.height();

    // for each peak...
    for (std::vector<PeakInfo>::const_iterator i = peaks.begin(); i != peaks.end(); ++i) {
        int x = i->x;
        int y = i->y;
        double peak = i->height;

        // ...extrapolation radius equals peak height
        int radius = static_cast<int>(peak);

        // ...determining bounding rectangle
        int left = max(x - radius, 0);
        int right = min(x + radius, hm_width - 1);
        int top = max(y - radius, 0);
        int bottom = min(y + radius, hm_height - 1);

        // ...for each point inside the rectangle
        for (int j = left; j <= right; ++j) {
            for (int k = top; k <= bottom; ++k) {
                // ...calculating distance to peak
                int vec_x = x - j;
                int vec_y = y - k;

                double vec_len = sqrt(static_cast<double>(vec_x * vec_x) +
                                      static_cast<double>(vec_y * vec_y));

                // ...if outside the circle, proceeding to the next iteration
                if (vec_len > peak)
                    continue;

                double h = peak - vec_len;                      // extrapolated value
                double curr_val = *(hm + j * hm_height + k);    // current value
                double h_diff = peak - curr_val;                // current-peak difference
                double add_val = h_diff / peak * h;             // correction value

                // ...modifying current value
                *(hm + j * hm_height + k) += add_val;
            }
        }

        raise_event(peakExtrapolated, PeakInfo, *i);
    }
}


void Mapper::calculateContours(
    const Landscape &ls,
    const std::vector<int> &levelLayout,
    std::list<Line2dSegment> &contours)
{
    using std::min;
    using std::max;

    const double *hm = ls.matrix();
    const int hm_width = ls.width();
    const int hm_height = ls.height();

    const int *levels = levelLayout.data();

    const int *const l_end = levels + levelLayout.size();

    const int min_lvl = min_level(levels, l_end);
    const int max_lvl = max_level(levels, l_end);

    for (int i = 1; i < hm_width; ++i) {
        for (int j = 1; j < hm_height; ++j) {
            // { i, j } is bottom right corner
            PeakInfo coords = { i, j };
            raise_event(contouringAt, PeakInfo, coords);

            int xVert[VERTEX_NUMBER];               // x-coordinates of rect vertices
            int yVert[VERTEX_NUMBER];               // y-coordinates of rect vertices
            double vertexVal[VERTEX_NUMBER + 1];    // values at vertices, [VERTEX_NUMBER] is a center

            bool isInRange = false;
            for (unsigned int v = 0; v < VERTEX_NUMBER; ++v) {  // retrieving local rect data
                xVert[v] = i + xShiftSequence[v];
                yVert[v] = j + yShiftSequence[v];
                vertexVal[v] = hm[xVert[v] * hm_height + yVert[v]];

                if (min_lvl <= vertexVal[v] && vertexVal[v] <= max_lvl)
                    isInRange = true;
            }

            if (!isInRange)     // skip rectangle, if all vertices are out of range
                continue;

            double xCenter = (xVert[0] + xVert[2]) / 2.0;
            double yCenter = (yVert[0] + yVert[2]) / 2.0;

            // Average center value
            vertexVal[VERTEX_NUMBER] = (vertexVal[0] + vertexVal[1] + vertexVal[2] + vertexVal[3]) / 4.0;

            double v2 = vertexVal[VERTEX_NUMBER];
            for (unsigned int t = 0; t < VERTEX_NUMBER; ++t) {  // each triangle
                int v1idx = v1Sequence[t];
                int v3idx = v3Sequence[t];

                double v1 = vertexVal[v1idx];
                double v3 = vertexVal[v3idx];

                for (const int *l = levels; l != l_end; ++l) {  // each level
                    double vMin = min(v2, min(v1, v3));
                    double vMax = max(v2, max(v1, v3));
                    if (!(vMin <= *l && *l <= vMax))
                        continue;

                    double x1;
                    double y1;
                    double x2;
                    double y2;

                    VertexZState v1state = vertexZState(v1, *l);
                    VertexZState v2state = vertexZState(v2, *l);
                    VertexZState v3state = vertexZState(v3, *l);

                    int vertexLayout = vertex_layout_map[v1state][v2state][v3state];
                    switch (vertexLayout)
                    {
                    default:
                    case 0:
                        continue;
                    case 1:     /* line between vertex 1 and vertex 2 */
                        x1 = xVert[v1idx];
                        y1 = yVert[v1idx];
                        x2 = xCenter;
                        y2 = yCenter;
                        break;
                    case 2:     /* line between vertex 2 and vertex 3 */
                        x1 = xCenter;
                        y1 = yCenter;
                        x2 = xVert[v3idx];
                        y2 = yVert[v3idx];
                        break;
                    case 3:     /* line between vertex 3 and vertex 1 */
                        x1 = xVert[v3idx];
                        y1 = yVert[v3idx];
                        x2 = xVert[v1idx];
                        y2 = yVert[v1idx];
                        break;
                    case 4:     /* line between vertex 1 and side 2-3 */
                        x1 = xVert[v1idx];
                        y1 = yVert[v1idx];
                        x2 = intersect_pos(xCenter, vertexVal[VERTEX_NUMBER], xVert[v3idx], vertexVal[v3idx], *l);
                        y2 = intersect_pos(yCenter, vertexVal[VERTEX_NUMBER], yVert[v3idx], vertexVal[v3idx], *l);
                        break;
                    case 5:     /* line between vertex 2 and side 3-1 */
                        x1 = xCenter;
                        y1 = yCenter;
                        x2 = intersect_pos(xVert[v3idx], vertexVal[v3idx], xVert[v1idx], vertexVal[v1idx], *l);
                        y2 = intersect_pos(yVert[v3idx], vertexVal[v3idx], yVert[v1idx], vertexVal[v1idx], *l);
                        break;
                    case 6:     /* line between vertex 3 and side 1-2 */
                        x1 = xVert[v3idx];
                        y1 = yVert[v3idx];
                        x2 = intersect_pos(xVert[v1idx], vertexVal[v1idx], xCenter, vertexVal[VERTEX_NUMBER], *l);
                        y2 = intersect_pos(yVert[v1idx], vertexVal[v1idx], yCenter, vertexVal[VERTEX_NUMBER], *l);
                        break;
                    case 7:     /* line between side 1-2 and side 2-3 */
                        x1 = intersect_pos(xVert[v1idx], vertexVal[v1idx], xCenter, vertexVal[VERTEX_NUMBER], *l);
                        y1 = intersect_pos(yVert[v1idx], vertexVal[v1idx], yCenter, vertexVal[VERTEX_NUMBER], *l);
                        x2 = intersect_pos(xCenter, vertexVal[VERTEX_NUMBER], xVert[v3idx], vertexVal[v3idx], *l);
                        y2 = intersect_pos(yCenter, vertexVal[VERTEX_NUMBER], yVert[v3idx], vertexVal[v3idx], *l);
                        break;
                    case 8:     /* line between side 2-3 and side 3-1 */
                        x1 = intersect_pos(xCenter, vertexVal[VERTEX_NUMBER], xVert[v3idx], vertexVal[v3idx], *l);
                        y1 = intersect_pos(yCenter, vertexVal[VERTEX_NUMBER], yVert[v3idx], vertexVal[v3idx], *l);
                        x2 = intersect_pos(xVert[v3idx], vertexVal[v3idx], xVert[v1idx], vertexVal[v1idx], *l);
                        y2 = intersect_pos(yVert[v3idx], vertexVal[v3idx], yVert[v1idx], vertexVal[v1idx], *l);
                        break;
                    case 9:     /* line between side 3-1 and side 1-2 */
                        x1 = intersect_pos(xVert[v3idx], vertexVal[v3idx], xVert[v1idx], vertexVal[v1idx], *l);
                        y1 = intersect_pos(yVert[v3idx], vertexVal[v3idx], yVert[v1idx], vertexVal[v1idx], *l);
                        x2 = intersect_pos(xVert[v1idx], vertexVal[v1idx], xCenter, vertexVal[VERTEX_NUMBER], *l);
                        y2 = intersect_pos(yVert[v1idx], vertexVal[v1idx], yCenter, vertexVal[VERTEX_NUMBER], *l);
                        break;
                    case 10:    /* all vertices are equal to the level */
                        /* store problem triangle coordinates */
                        continue;
                    }

                    Line2dSegment segment = { x1, y1, x2, y2, *l };
                    contours.push_back(segment);
                } // for l
            } // for t
        } // for j
    } // for i
}


Mapper::~Mapper() { }


} // namespace HeightMap
