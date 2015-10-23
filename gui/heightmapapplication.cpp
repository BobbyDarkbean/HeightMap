#include <QDir>
#include <QPluginLoader>
#include <QSettings>
#include "preferences.h"
#include "preferencescontroller.h"
#include "extrapolationdata.h"
#include "extrapolationfactory.h"
#include "heightmaplogic.h"

#include "heightmapapplication.h"


namespace HeightMap {


struct HeightMapApplicationImplementation
{
    HeightMapApplicationImplementation();

    void importExtrapolations();
    void cleanupExtrapolations();

    void loadPreferences();
    void savePreferences();

    void addExtrapolation(ExtrapolationFactory *);

    ~HeightMapApplicationImplementation();

    HeightMapLogic *logic;

    Preferences prefs;
    PreferencesController *ctrl;
    QMap<QString, ExtrapolationFactory *> extrapolations;

    const QString IniFilename;
    const QString XDataPath;

private:
    DISABLE_COPY(HeightMapApplicationImplementation)
    DISABLE_MOVE(HeightMapApplicationImplementation)
};


HeightMapApplicationImplementation::HeightMapApplicationImplementation()
    : logic(nullptr),
      prefs(),
      ctrl(nullptr),
      extrapolations(),
      IniFilename(QApplication::applicationDirPath() + "/hmcfg.ini"),
      XDataPath(QApplication::applicationDirPath() + "/xdata")
{
    loadPreferences();
    importExtrapolations();
}

void HeightMapApplicationImplementation::importExtrapolations()
{
    QDir xDataDir(XDataPath);

    QStringList pluginFilter;
    pluginFilter << "*hmxplg*.*";
    QStringList pluginNames = xDataDir.entryList(pluginFilter, QDir::Files);

    foreach (const QString &filename, pluginNames) {
        QPluginLoader loader(xDataDir.absoluteFilePath(filename));
        if (ExtrapolationFactory *f = qobject_cast<ExtrapolationFactory *>(loader.instance()))
            addExtrapolation(f);
    }
}

void HeightMapApplicationImplementation::cleanupExtrapolations()
{
    for (auto i = extrapolations.constBegin(); i != extrapolations.constEnd(); ++i) {
        delete i.value();
    }
}

void HeightMapApplicationImplementation::loadPreferences()
{
    QSettings settings(IniFilename, QSettings::IniFormat);
    prefs.setLandscapeWidth(qvariant_cast<int>(settings.value("landscape/width", Preferences::DefaultLandscapeWidth)));
    prefs.setLandscapeHeight(qvariant_cast<int>(settings.value("landscape/height", Preferences::DefaultLandscapeHeight)));
    prefs.setPeakCount(qvariant_cast<unsigned int>(settings.value("peaks/count", Preferences::DefaultPeakCount)));
    prefs.setMinPeak(qvariant_cast<int>(settings.value("peaks/minpeak", Preferences::DefaultMinPeak)));
    prefs.setMaxPeak(qvariant_cast<int>(settings.value("peaks/maxpeak", Preferences::DefaultMaxPeak)));
    prefs.setExtrapolatorName(qvariant_cast<QString>(settings.value("extrapolation/name", QString())));
    prefs.setMinContouringLevel(qvariant_cast<int>(settings.value("contouring/minlevel", Preferences::DefaultMinContourLevel)));
    prefs.setMaxContouringLevel(qvariant_cast<int>(settings.value("contouring/maxlevel", Preferences::DefaultMaxContourLevel)));
    prefs.setContouringStep(qvariant_cast<int>(settings.value("contouring/step", Preferences::DefaultContouringStep)));
    prefs.setImageFactor(qvariant_cast<int>(settings.value("appearance/imgfactor", Preferences::DefaultImageFactor)));

    settings.sync();
}

void HeightMapApplicationImplementation::savePreferences()
{
    QSettings settings(IniFilename, QSettings::IniFormat);

    settings.beginGroup("landscape");
    settings.setValue("width", prefs.landscapeWidth());
    settings.setValue("height", prefs.landscapeHeight());
    settings.endGroup();

    settings.beginGroup("peaks");
    settings.setValue("count", prefs.peakCount());
    settings.setValue("minpeak", prefs.minPeak());
    settings.setValue("maxpeak", prefs.maxPeak());
    settings.endGroup();

    settings.beginGroup("extrapolation");
    settings.setValue("name", prefs.extrapolatorName());
    settings.endGroup();

    settings.beginGroup("contouring");
    settings.setValue("minlevel", prefs.minContouringLevel());
    settings.setValue("maxlevel", prefs.maxContouringLevel());
    settings.setValue("step", prefs.contouringStep());
    settings.endGroup();

    settings.beginGroup("appearance");
    settings.setValue("imgfactor", prefs.imageFactor());
    settings.endGroup();
}

void HeightMapApplicationImplementation::addExtrapolation(ExtrapolationFactory *f)
{ extrapolations.insert(f->name(), f); }

HeightMapApplicationImplementation::~HeightMapApplicationImplementation()
{
    cleanupExtrapolations();
    savePreferences();
}


HeightMapApplication::HeightMapApplication(int &argc, char **argv)
    : QApplication(argc, argv),
      m(new HeightMapApplicationImplementation)
{
    m->logic = new HeightMapLogic(this);
    m->logic->setPreferences(m->prefs);

    m->ctrl = new PreferencesController(this);
    m->ctrl->setPreferences(&m->prefs);

    typedef HeightMapLogic L;
    typedef HeightMapApplication A;

    connect(m->logic,   &L::preferencesChanged,         this,   &A::setPreferences);
    connect(m->logic,   &L::extrapolationDataChanged,   this,   &A::setXData);
}


HeightMapLogic *HeightMapApplication::logic() const
{ return m->logic; }

const Preferences &HeightMapApplication::preferences() const
{ return m->prefs; }

void HeightMapApplication::setPreferences(const Preferences &prefs)
{
    if (m->prefs != prefs) {
        m->prefs = prefs;
        emit preferencesChanged();
    }
}

PreferencesController *HeightMapApplication::preferencesController() const
{ return m->ctrl; }

ExtrapolationData HeightMapApplication::xData(const QString &name) const
{
    if (ExtrapolationFactory *f = extrapolationFactory(name))
        return f->extractData();

    return ExtrapolationData();
}

void HeightMapApplication::setXData(const QString &name, const ExtrapolationData &xdata)
{
    if (ExtrapolationFactory *f = extrapolationFactory(name)) {
        f->provideData(xdata);
        emit extrapolationDataChanged(name);
    }
}

QStringList HeightMapApplication::extrapolatorKeys() const
{ return m->extrapolations.keys(); }

ExtrapolationFactory *HeightMapApplication::extrapolationFactory(const QString &name) const
{ return m->extrapolations.value(name, nullptr); }

ExtrapolationFactory *HeightMapApplication::currentExtrapolationFactory() const
{ return extrapolationFactory(preferences().extrapolatorName()); }


HeightMapApplication::~HeightMapApplication()
{
    delete m;
}


} // namespace HeightMap
