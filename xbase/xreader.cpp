#include <QFile>
#include <QXmlStreamReader>
#include "extrapolationdata.h"

#include "xreader.h"


namespace HeightMap {


struct XReaderImplementation
{
    explicit XReaderImplementation(const QString &filename);
    ~XReaderImplementation();

    bool valid;
    ExtrapolationData xdata;

private:
    DISABLE_COPY(XReaderImplementation)
    DISABLE_MOVE(XReaderImplementation)
};


XReaderImplementation::XReaderImplementation(const QString &filename)
    : valid(false),
      xdata()
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QXmlStreamReader xsr(&file);

    while (!xsr.atEnd() && !xsr.isStartDocument())
        xsr.readNext();

    if (!xsr.isStartDocument()) {
        xsr.raiseError("Awaiting XML-declaration.");
        return;
    }

    while (!xsr.atEnd() && !xsr.isStartElement())
        xsr.readNext();

    if (!xsr.isStartElement()) {
        xsr.raiseError("Awaiting root XML element.");
        return;
    } else if (xsr.name() != "xdata") {
        xsr.raiseError("Invalid root element.");
        return;
    }

    while (!xsr.atEnd()) {
        if (xsr.readNext() == QXmlStreamReader::StartElement) {
            QString name = xsr.name().toString();
            QString valueStr = xsr.attributes().value("value").toString();

            bool dConvert = false;
            double value = valueStr.toDouble(&dConvert);
            if (!dConvert) {
                xsr.raiseError("Can not parse floating-point value.");
            }

            xdata.insert(name, value);
        }
    }

    valid = !xsr.hasError();
}

XReaderImplementation::~XReaderImplementation() { }


XReader::XReader(const QString &filename)
    : m(new XReaderImplementation(filename)) { }


ExtrapolationData XReader::data() const
{ return m->xdata; }

bool XReader::isValid() const
{ return m->valid; }


XReader::~XReader()
{
    delete m;
}


} // namespace HeightMap
