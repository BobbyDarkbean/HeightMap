#include <QMap>
#include <QDataStream>
#include "hmdef.h"

#include "extrapolationdata.h"


namespace HeightMap {


struct ExtrapolationDataImplementation
{
    ExtrapolationDataImplementation();

    bool content_equals(const ExtrapolationDataImplementation &);
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

bool ExtrapolationDataImplementation::content_equals(const ExtrapolationDataImplementation &other)
{ return xMap == other.xMap; }

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


bool ExtrapolationData::isEmpty() const
{ return m->xMap.isEmpty(); }

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


void ExtrapolationData::writeTo(QDataStream &stream) const
{
    stream << m->xMap;
}

void ExtrapolationData::readFrom(QDataStream &stream)
{
    detach();
    stream >> m->xMap;
}


bool ExtrapolationData::equals(const ExtrapolationData &other) const
{ return m == other.m ? true : m->content_equals(*other.m); }

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


QDataStream &operator <<(QDataStream &stream, const ExtrapolationData &xdata)
{
    xdata.writeTo(stream);
    return stream;
}

QDataStream &operator >>(QDataStream &stream, ExtrapolationData &xdata)
{
    xdata.readFrom(stream);
    return stream;
}


} // namespace HeightMap
