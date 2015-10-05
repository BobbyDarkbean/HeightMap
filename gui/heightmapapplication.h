#ifndef _HeightMapApplication_h_
#define _HeightMapApplication_h_


#include <QApplication>
#include "hm_shared.h"


namespace HeightMap {


class AbstractExtrapolationWidget;
class HeightMapLogic;
struct HeightMapApplicationImplementation;
class HeightMapApplication : public QApplication
{
    Q_OBJECT

public:
    HeightMapApplication(int &argc, char **argv);

    HeightMapLogic *logic() const;

    ~HeightMapApplication();

private:
    DISABLE_COPY(HeightMapApplication)
    DISABLE_MOVE(HeightMapApplication)

    HeightMapApplicationImplementation *m;
};


} // namespace HeightMap


#endif // _HeightMapApplication_h_
