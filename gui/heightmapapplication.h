#ifndef _HeightMapApplication_h_
#define _HeightMapApplication_h_


#include <QApplication>
#include "hm_shared.h"


namespace HeightMap {


class Preferences;
struct HeightMapApplicationImplementation;
class HeightMapApplication : public QApplication
{
    Q_OBJECT

public:
    HeightMapApplication(int &argc, char **argv);

    const Preferences &preferences() const;
    void setPreferences(const Preferences &);

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
