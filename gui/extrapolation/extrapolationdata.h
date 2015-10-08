#ifndef _ExtrapolationData_h_
#define _ExtrapolationData_h_


#include <QString>


namespace HeightMap {


struct ExtrapolationDataImplementation;
class ExtrapolationData
{
public:
    ExtrapolationData();
    ExtrapolationData(const ExtrapolationData &);
    ExtrapolationData(ExtrapolationData &&);

    QStringList keys() const;

    void insert(const QString &key, double value);
    void remove(const QString &key);
    double value(const QString &key, double defaultValue) const;

    void detach();
    void swap(ExtrapolationData &);

    ExtrapolationData &operator=(ExtrapolationData);    // copy-and-swap
    ~ExtrapolationData();

private:
    ExtrapolationDataImplementation *m;
};


} // namespace HeightMap


namespace std {


template <>
inline void swap<HeightMap::ExtrapolationData>(
    HeightMap::ExtrapolationData &a,
    HeightMap::ExtrapolationData &b) { a.swap(b); }


} // namespace std


#endif // _ExtrapolationData_h_
