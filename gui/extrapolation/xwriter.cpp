#include <QMap>
#include <QFile>
#include <QXmlStreamWriter>

#include "xwriter.h"


namespace HeightMap {


struct XWriterImplementation
{
    explicit XWriterImplementation(const QString &filename);
    ~XWriterImplementation();

    QString fname;
    QMap<QString, double> xdata;

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

    for (auto i = xdata.constBegin(); i != xdata.constEnd(); ++i) {
        xsw.writeEmptyElement(i.key());
        xsw.writeAttribute("value", QString::number(i.value(), 'f', 2));
    }

    xsw.writeEndElement();

    xsw.writeEndDocument();
}


XWriter::XWriter(const QString &filename)
    : m(new XWriterImplementation(filename)) { }


void XWriter::writeElement(
    const QString &name,
    double value)
{ m->xdata.insert(name, value); }


XWriter::~XWriter()
{
    delete m;
}


} // namespace HeightMap
