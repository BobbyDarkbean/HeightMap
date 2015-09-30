#ifndef _PreferencesController_h_
#define _PreferencesController_h_


#include <QObject>
#include "hm_shared.h"


namespace HeightMap {


class Preferences;
class PreferencesController : public QObject
{
    Q_OBJECT

public:
    explicit PreferencesController(QObject *parent = 0);

    Preferences *preferences() const;
    void setPreferences(Preferences *);

    ~PreferencesController();

public slots:
    void setLandscapeWidth(int);
    void setLandscapeHeight(int);
    void setPeakCount(int);
    void setMinPeak(int);
    void setMaxPeak(int);
    void setExtrapolatorName(QString);
    void setMinContouringLevel(int);
    void setMaxContouringLevel(int);
    void setContouringStep(int);
    void setImageFactor(int);

private:
    DISABLE_COPY(PreferencesController)
    DISABLE_MOVE(PreferencesController)

    Preferences *m_prefs;
};


} // namespace HeightMap


#endif // _PreferencesController_h_
