#include <QFile>
#include <QXmlStreamWriter>
#include <QStringList>
#include "extrapolationdata.h"

#include "xwriter.h"


namespace HeightMap {


struct XWriterImplementation
{
    explicit XWriterImplementation(const QString &filename);
    ~XWriterImplementation();

    QString fname;
    ExtrapolationData xdata;

private:
    DISABLE_COPY(XWriterImplementation)
    DISABLE_MOVE(XWriterImplementation)
};


XWriterImplementation::XWriterImplementation(const QString &filename)
    : fname(filename),
      xdata() { }

XWriterImplementation::~XWriterImplementation()
{
    QFile file(fname);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        return;
    }

    QXmlStreamWriter xsw(&file);
    xsw.setAutoFormatting(true);
    xsw.writeStartDocument();

    xsw.writeStartElement("xdata");

    foreach (QString s, xdata.keys()) {
        xsw.writeEmptyElement(s);
        xsw.writeAttribute("value", QString::number(xdata.value(s, 0.0), 'f', 2));
    }

    xsw.writeEndElement();

    xsw.writeEndDocument();
}


XWriter::XWriter(const QString &filename)
    : m(new XWriterImplementation(filename)) { }


ExtrapolationData XWriter::data() const
{ return m->xdata; }

void XWriter::setData(const ExtrapolationData &d)
{ m->xdata = d; }


XWriter::~XWriter()
{
    delete m;
}


} // namespace HeightMap
