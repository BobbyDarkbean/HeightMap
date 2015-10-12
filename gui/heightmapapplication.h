#ifndef _HeightMapApplication_h_
#define _HeightMapApplication_h_


#include <QApplication>
#include "hm_shared.h"


namespace HeightMap {


class AbstractExtrapolationWidget;
class HeightMapLogic;
class Preferences;
class PreferencesController;
class ExtrapolationFactory;
class ExtrapolationData;
struct HeightMapApplicationImplementation;
class HeightMapApplication : public QApplication
{
    Q_OBJECT

public:
    HeightMapApplication(int &argc, char **argv);

    HeightMapLogic *logic() const;

    const Preferences &preferences() const;
    void setPreferences(const Preferences &);
    PreferencesController *preferencesController() const;

    ExtrapolationData xData(const QString &name) const;
    void setXData(const QString &, const ExtrapolationData &);

    QStringList extrapolatorKeys() const;
    ExtrapolationFactory *extrapolationFactory(const QString &name) const;
    ExtrapolationFactory *currentExtrapolationFactory() const;

    ~HeightMapApplication();

signals:
    void preferencesChanged();
    void extrapolationDataChanged(QString);

private:
    DISABLE_COPY(HeightMapApplication)
    DISABLE_MOVE(HeightMapApplication)

    HeightMapApplicationImplementation *m;
};


} // namespace HeightMap


#define hmApp static_cast< ::HeightMap::HeightMapApplication *>(qApp)


#endif // _HeightMapApplication_h_
