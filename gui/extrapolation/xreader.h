#ifndef _XReader_h_
#define _XReader_h_


#include "hm_shared.h"


class QString;
namespace HeightMap {


struct XReaderImplementation;
class XReader
{
public:
    explicit XReader(const QString &filename);

    bool isValid() const;
    double readElement(const QString &name, double defaultValue) const;

    ~XReader();

private:
    DISABLE_COPY(XReader)
    DISABLE_MOVE(XReader)

    XReaderImplementation *m;
};


} // namespace HeightMap


#endif // _XReader_h_
