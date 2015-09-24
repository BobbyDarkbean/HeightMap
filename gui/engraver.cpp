#include <QPainter>
#include <QImage>
#include "landscape.h"
#include "colormap.h"

#include "engraver.h"


namespace HeightMap {


namespace {
inline QColor colorForLevel(int level)
{
    return QColor(COLOR_MAP[level][0],
                  COLOR_MAP[level][1],
                  COLOR_MAP[level][2]);
}
}


Engraver::Engraver()
{
}

void Engraver::drawPeaks(
    const std::vector<HeightMap::PeakInfo> &peaks,
    QImage *image,
    int imageFactor)
{
    const int Shift = imageFactor / 2;

    const double Radius = static_cast<double>(imageFactor * 2);
    const double Diameter = Radius * 2;

    QPainter p(image);
    p.setRenderHint(QPainter::Antialiasing, true);

    std::vector<PeakInfo>::const_iterator _end = peaks.end();
    for (std::vector<PeakInfo>::const_iterator i = peaks.begin(); i != _end; ++i) {
        QPointF center(static_cast<double>(i->x * imageFactor + Shift),
                       static_cast<double>(i->y * imageFactor + Shift));
        QRadialGradient grad(center, Radius);
        grad.setColorAt(0.0, colorForLevel(static_cast<int>(i->height)));
        grad.setColorAt(1.0, Qt::transparent);

        QRectF rect(center - QPointF(Radius, Radius),
                    QSizeF(Diameter, Diameter));

        p.setPen(Qt::NoPen);
        p.setBrush(grad);
        p.drawRect(rect);

        QPoint points[] = {
            QPoint(i->x * imageFactor,
                   i->y * imageFactor),
            QPoint(i->x * imageFactor + imageFactor,
                   i->y * imageFactor),
            QPoint(i->x * imageFactor,
                   i->y * imageFactor + imageFactor)
        };

        p.setBrush(Qt::black);
        p.drawPolygon(points, 3);
    }
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
            p.setBrush(colorForLevel(level));
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
        p->setPen(QPen(colorForLevel(i->level), 1.0));
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
