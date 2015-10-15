#ifndef _ExtrapolationOptionsWidget_h_
#define _ExtrapolationOptionsWidget_h_


#include <QWidget>
#include "hmdef.h"
#include "../ge_shared.h"


namespace HeightMap {


class ExtrapolationData;
class ExtrapolationFactory;
struct ExtrapolationOptionsWidgetImplementation;
class HMGE_SHARED ExtrapolationOptionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExtrapolationOptionsWidget(QWidget *parent = 0);

    QString extrapolatorName() const;
    void setExtrapolatorName(const QString &);

    ExtrapolationData xData(const QString &name);
    void setXData(const QString &name, const ExtrapolationData &xdata);

    QWidget *extrapolationWidget(const QString &name) const;
    void addExtrapolationWidget(ExtrapolationFactory *, bool bound);
    void removeExtrapolationWidget(const QString &name);

    void retrieveExtrapolationSettings();

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
