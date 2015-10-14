#ifndef _XReader_h_
#define _XReader_h_


#include "hmdef.h"


class QString;
namespace HeightMap {


class ExtrapolationData;
struct XReaderImplementation;
class XReader
{
public:
    explicit XReader(const QString &filename);

    ExtrapolationData data() const;
    bool isValid() const;

    ~XReader();

private:
    DISABLE_COPY(XReader)
    DISABLE_MOVE(XReader)

    XReaderImplementation *m;
};


} // namespace HeightMap


#endif // _XReader_h_
