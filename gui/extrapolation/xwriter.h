#ifndef _XWriter_h_
#define _XWriter_h_


#include "hm_shared.h"


class QString;
namespace HeightMap {


struct XWriterImplementation;
class XWriter
{
public:
    explicit XWriter(const QString &filename);
    void writeElement(const QString &name, double value);
    ~XWriter();

private:
    DISABLE_COPY(XWriter)
    DISABLE_MOVE(XWriter)

    XWriterImplementation *m;
};


} // namespace HeightMap


#endif // _XWriter_h_
