#ifndef _XWriter_h_
#define _XWriter_h_


#include "xbase_shared.h"
#include "hmdef.h"


class QString;
namespace HeightMap {


class ExtrapolationData;
struct XWriterImplementation;
class HMX_SHARED XWriter
{
public:
    explicit XWriter(const QString &filename);

    ExtrapolationData data() const;
    void setData(const ExtrapolationData &);

    ~XWriter();

private:
    DISABLE_COPY(XWriter)
    DISABLE_MOVE(XWriter)

    XWriterImplementation *m;
};


} // namespace HeightMap


#endif // _XWriter_h_
