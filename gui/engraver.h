#ifndef _Engraver_h_
#define _Engraver_h_


#include <list>
#include "line2dsegment.h"


class QPainter;
class QImage;


namespace HeightMap {


class Landscape;
class Engraver
{
public:
    Engraver();

    void drawLandscape(
        const Landscape &ls,
        QImage *image,
        int imageFactor);
    void drawIsobars(
        const std::list<Line2dSegment> &isobars,
        QImage *image,
        bool coloured,
        int imageFactor);

private:
    void drawColouredIsobars(
        const std::list<Line2dSegment> &isobars,
        QPainter *,
        int imageFactor);
    void drawMonochromeIsobars(
        const std::list<Line2dSegment> &isobars,
        QPainter *,
        int imageFactor);
};


} // namespace HeightMap


#endif // _Engraver_h_
