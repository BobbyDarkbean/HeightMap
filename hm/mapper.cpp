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
            rand() % static_cast<int>(opts.hmWidth),
            rand() % static_cast<int>(opts.hmHeight),
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

    double *hm = ls.matrix();
    unsigned int hm_width = ls.width();
    unsigned int hm_height = ls.height();

    // for each peak...
    for (std::vector<PeakInfo>::const_iterator i = peaks.begin(); i != peaks.end(); ++i) {
        int x = i->x;
        int y = i->y;
        double peak = i->height;

        int radius = static_cast<int>(peak);

        int left = std::max(x - radius, 0);
        int right = std::min(x + radius, static_cast<int>(hm_width) - 1);
        int top = std::max(y - radius, 0);
        int bottom = std::min(y + radius, static_cast<int>(hm_height) - 1);

        for (int j = left; j <= right; ++j) {
            for (int k = top; k <= bottom; ++k) {
                int vec_x = x - j;
                int vec_y = y - k;

                double vec_len = std::sqrt(static_cast<double>(vec_x * vec_x) +
                                           static_cast<double>(vec_y * vec_y));

                if (vec_len > peak)
                    continue;

                double h = peak - vec_len;
                double curr_val = *(hm + j * hm_height + k);
                double h_diff = peak - curr_val;
                double add_val = h_diff / peak * h;

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
