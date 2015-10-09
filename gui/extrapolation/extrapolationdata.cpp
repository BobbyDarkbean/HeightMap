#include <QMap>
#include <QStringList>
#include "hm_shared.h"

#include "extrapolationdata.h"


namespace HeightMap {


struct ExtrapolationDataImplementation
{
    ExtrapolationDataImplementation();
    ExtrapolationDataImplementation *clone() const;
    ~ExtrapolationDataImplementation();

    QMap<QString, double> xMap;
    unsigned int ref;

private:
    // private copy constructor for cloning
    ExtrapolationDataImplementation(const ExtrapolationDataImplementation &);
    // other copy-move methods are disabled
    ExtrapolationDataImplementation &operator=(const ExtrapolationDataImplementation &) = delete;
    DISABLE_MOVE(ExtrapolationDataImplementation)
};


ExtrapolationDataImplementation::ExtrapolationDataImplementation()
    : xMap(),
      ref(1) { }

ExtrapolationDataImplementation *ExtrapolationDataImplementation::clone() const
{ return new ExtrapolationDataImplementation(*this); }

ExtrapolationDataImplementation::~ExtrapolationDataImplementation() { }

// private
ExtrapolationDataImplementation::ExtrapolationDataImplementation(const ExtrapolationDataImplementation &other)
    : xMap(other.xMap),
      ref(1) { }


ExtrapolationData::ExtrapolationData()
    : m(new ExtrapolationDataImplementation) { }

ExtrapolationData::ExtrapolationData(const ExtrapolationData &other)
    : m(other.m)
{
    ++m->ref;
}

ExtrapolationData::ExtrapolationData(ExtrapolationData &&other)
    : m(other.m)
{
    other.m = nullptr;
}


QStringList ExtrapolationData::keys() const
{ return m->xMap.keys(); }

void ExtrapolationData::insert(const QString &key, double value)
{
    detach();
    m->xMap.insert(key, value);
}

void ExtrapolationData::remove(const QString &key)
{
    detach();
    m->xMap.remove(key);
}

double ExtrapolationData::value(const QString &key, double defaultValue) const
{ return m->xMap.value(key, defaultValue); }


void ExtrapolationData::detach()
{
    if (m->ref > 1) {
        --m->ref;
        m = m->clone();
    }
}

void ExtrapolationData::swap(ExtrapolationData &other)
{
    using std::swap;
    swap(m, other.m);
}

ExtrapolationData &ExtrapolationData::operator=(ExtrapolationData other)
{
    swap(other);
    return *this;
}

ExtrapolationData::~ExtrapolationData()
{
    if (m) {
        if (--m->ref == 0)
            delete m;
    }
}


} // namespace HeightMap
