#ifndef _ExtrapolationData_h_
#define _ExtrapolationData_h_


#include <algorithm>
#include <QString>
#include "xbase_shared.h"


class QDataStream;
namespace HeightMap {


struct ExtrapolationDataImplementation;
class HMX_SHARED ExtrapolationData
{
public:
    ExtrapolationData();
    ExtrapolationData(const ExtrapolationData &);
    ExtrapolationData(ExtrapolationData &&);

    bool isEmpty() const;

    QStringList keys() const;

    void insert(const QString &key, double value);
    void remove(const QString &key);
    double value(const QString &key, double defaultValue) const;

    void writeTo(QDataStream &) const;
    void readFrom(QDataStream &);

    bool equals(const ExtrapolationData &) const;

    void detach();
    void swap(ExtrapolationData &);

    ExtrapolationData &operator=(ExtrapolationData);    // copy-and-swap
    ~ExtrapolationData();

private:
    ExtrapolationDataImplementation *m;
};


HMX_SHARED QDataStream &operator <<(QDataStream &, const ExtrapolationData &);
HMX_SHARED QDataStream &operator >>(QDataStream &, ExtrapolationData &);


inline bool operator ==(const ExtrapolationData &a, const ExtrapolationData &b)
{ return a.equals(b); }

inline bool operator !=(const ExtrapolationData &a, const ExtrapolationData &b)
{ return !(a == b); }


} // namespace HeightMap


namespace std {


template <>
inline void swap<HeightMap::ExtrapolationData>(
    HeightMap::ExtrapolationData &a,
    HeightMap::ExtrapolationData &b) { a.swap(b); }


} // namespace std


#endif // _ExtrapolationData_h_
