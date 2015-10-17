#include <QEvent>
#include "abstractextrapolationwidget.h"


namespace HeightMap {


AbstractExtrapolationWidget::AbstractExtrapolationWidget(QWidget *parent)
    : QWidget(parent) { }


bool AbstractExtrapolationWidget::eventFilter(QObject *o, QEvent *e)
{
    const QEvent::Type t = e->type();
    switch (t) {
    case QEvent::ShortcutOverride:
        return true;
    default:
        break;
    }

    return QWidget::eventFilter(o, e);
}


AbstractExtrapolationWidget::~AbstractExtrapolationWidget() { }


} // namespace HeightMap
