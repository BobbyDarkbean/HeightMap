#include <QPainter>
#include <QImage>
#include "landscape.h"
#include "colormap.h"

#include "engraver.h"


namespace HeightMap {


Engraver::Engraver()
{
}

void Engraver::drawLandscape(
    const Landscape &ls,
    QImage *image,
    int imageFactor)
{
    QPainter p(image);
    p.setPen(Qt::NoPen);

    const double *d = ls.matrix();
    int w = ls.width();
    int h = ls.height();

    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j, ++d) {
            int level = static_cast<int>(*d);
            p.setBrush(QColor(COLOR_MAP[level][0],
                              COLOR_MAP[level][1],
                              COLOR_MAP[level][2]));
            p.drawRect(i * imageFactor, j * imageFactor, imageFactor, imageFactor);
        }
    }
}

void Engraver::drawIsobars(
    const std::list<Line2dSegment> &isobars,
    QImage *image,
    bool coloured,
    int imageFactor)
{
    QPainter p(image);
    p.setBrush(Qt::NoBrush);

    coloured ? drawColouredIsobars(isobars, &p, imageFactor)
             : drawMonochromeIsobars(isobars, &p, imageFactor);
}


void Engraver::drawColouredIsobars(
    const std::list<Line2dSegment> &isobars,
    QPainter *p,
    int imageFactor)
{
    const int Shift = imageFactor / 2;

    std::list<Line2dSegment>::const_iterator _end = isobars.end();
    for (std::list<Line2dSegment>::const_iterator i = isobars.begin(); i != _end; ++i) {
        p->setPen(QPen(QColor(COLOR_MAP[i->level][0],
                              COLOR_MAP[i->level][1],
                              COLOR_MAP[i->level][2]),
                       1.0));
        p->drawLine(i->x1 * imageFactor + Shift,
                    i->y1 * imageFactor + Shift,
                    i->x2 * imageFactor + Shift,
                    i->y2 * imageFactor + Shift);
    }
}

void Engraver::drawMonochromeIsobars(
    const std::list<Line2dSegment> &isobars,
    QPainter *p,
    int imageFactor)
{
    const int Shift = imageFactor / 2;
    p->setPen(QPen(Qt::black, 1.0));

    std::list<Line2dSegment>::const_iterator _end = isobars.end();
    for (std::list<Line2dSegment>::const_iterator i = isobars.begin(); i != _end; ++i) {
        p->drawLine(i->x1 * imageFactor + Shift,
                    i->y1 * imageFactor + Shift,
                    i->x2 * imageFactor + Shift,
                    i->y2 * imageFactor + Shift);
    }
}


} // namespace HeightMap
