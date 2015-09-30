#ifndef _ExtrapolationOptionsWidget_h_
#define _ExtrapolationOptionsWidget_h_


#include <QWidget>
#include "hm_shared.h"


namespace HeightMap {


class AbstractExtrapolationWidget;
struct ExtrapolationOptionsWidgetImplementation;
class ExtrapolationOptionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExtrapolationOptionsWidget(QWidget *parent = 0);

    QSize sizeHint() const;

    QString extrapolatorName() const;
    void setExtrapolatorName(const QString &);

    QWidget *extrapolationWidget(const QString &name) const;
    void addExtrapolationWidget(
        const QString &name,
        const QString &description,
        AbstractExtrapolationWidget *);

    void acceptExtrapolationSettings();

    ~ExtrapolationOptionsWidget();

signals:
    void extrapolatorNameChanged(QString);

private slots:
    void setExtrapolationWidget(int);

private:
    DISABLE_COPY(ExtrapolationOptionsWidget)
    DISABLE_MOVE(ExtrapolationOptionsWidget)

    ExtrapolationOptionsWidgetImplementation *m;
};


} // namespace HeightMap


#endif // _ExtrapolationOptionsWidget_h_
