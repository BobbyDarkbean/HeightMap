#ifndef _HeightMapApplication_h_
#define _HeightMapApplication_h_


#include <QApplication>
#include "hm_shared.h"


namespace HeightMap {


class Preferences;
class PreferencesController;
class Extrapolator;
class AbstractExtrapolationWidget;
class HeightMapLogic;
struct HeightMapApplicationImplementation;
class HeightMapApplication : public QApplication
{
    Q_OBJECT

public:
    HeightMapApplication(int &argc, char **argv);

    const Preferences &preferences() const;
    void setPreferences(const Preferences &);

    PreferencesController *preferencesController() const;

    HeightMapLogic *logic() const;

    QStringList extrapolatorKeys() const;

    AbstractExtrapolationWidget *createExtrapolationWidget(const QString &name) const;
    QString extrapolationDescription(const QString &name) const;
    Extrapolator *currentExtrapolator() const;

    ~HeightMapApplication();

signals:
    void preferencesChanged();

private:
    DISABLE_COPY(HeightMapApplication)
    DISABLE_MOVE(HeightMapApplication)

    HeightMapApplicationImplementation *m;
};


} // namespace HeightMap


#define hmApp static_cast< ::HeightMap::HeightMapApplication *>(qApp)


#endif // _HeightMapApplication_h_
