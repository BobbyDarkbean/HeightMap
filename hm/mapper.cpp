#include <cstdlib>
#include <ctime>
#include <cmath>
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

        peaks.push_back(peak);
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

}


Mapper::~Mapper() { }


} // namespace HeightMap
