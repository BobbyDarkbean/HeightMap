#include <QSettings>
#include "preferences.h"

#include "heightmapapplication.h"


namespace HeightMap {


struct HeightMapApplicationImplementation
{
    HeightMapApplicationImplementation();
    ~HeightMapApplicationImplementation();

    Preferences prefs;
    const QString IniFilename;

private:
    DISABLE_COPY(HeightMapApplicationImplementation)
    DISABLE_MOVE(HeightMapApplicationImplementation)
};


HeightMapApplicationImplementation::HeightMapApplicationImplementation()
    : prefs(),
      IniFilename(QApplication::applicationDirPath() + "/hmcfg.ini")
{
    QSettings settings(IniFilename, QSettings::IniFormat);
    prefs.setLandscapeWidth(qvariant_cast<int>(settings.value("landscape/width", Preferences::DefaultLandscapeWidth)));
    prefs.setLandscapeHeight(qvariant_cast<int>(settings.value("landscape/height", Preferences::DefaultLandscapeHeight)));
    prefs.setPeakCount(qvariant_cast<unsigned int>(settings.value("peaks/count", Preferences::DefaultPeakCount)));
    prefs.setMinPeak(qvariant_cast<int>(settings.value("peaks/minpeak", Preferences::DefaultMinPeak)));
    prefs.setMaxPeak(qvariant_cast<int>(settings.value("peaks/maxpeak", Preferences::DefaultMaxPeak)));
    prefs.setLandscapeBase(qvariant_cast<int>(settings.value("extrapolation/base", Preferences::DefaultLandscapeBase)));
    prefs.setMinContouringLevel(qvariant_cast<int>(settings.value("contouring/minlevel", Preferences::DefaultMinContourLevel)));
    prefs.setMaxContouringLevel(qvariant_cast<int>(settings.value("contouring/maxlevel", Preferences::DefaultMaxContourLevel)));
    prefs.setContouringStep(qvariant_cast<int>(settings.value("contouring/step", Preferences::DefaultContouringStep)));
    prefs.setImageFactor(qvariant_cast<int>(settings.value("appearance/imgfactor", Preferences::DefaultImageFactor)));

    settings.sync();
}

HeightMapApplicationImplementation::~HeightMapApplicationImplementation()
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
    settings.setValue("base", prefs.landscapeBase());
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


HeightMapApplication::HeightMapApplication(int &argc, char **argv)
    : QApplication(argc, argv),
      m(new HeightMapApplicationImplementation) { }


const Preferences &HeightMapApplication::preferences() const
{ return m->prefs; }

void HeightMapApplication::setPreferences(const Preferences &prefs)
{
    if (m->prefs != prefs) {
        m->prefs = prefs;
        emit preferencesChanged();
    }
}


HeightMapApplication::~HeightMapApplication()
{
    delete m;
}


} // namespace HeightMap
