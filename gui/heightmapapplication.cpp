#include <QSettings>
#include "preferences.h"
#include "extrapolation/simpleextrapolationfactory.h"
#include "extrapolation/slopeextrapolationfactory.h"
#include "extrapolation/baselevelextrapolationfactory.h"
#include "extrapolation/fixedradiusextrapolationfactory.h"
#include "heightmaplogic.h"

#include "heightmapapplication.h"


namespace HeightMap {


struct HeightMapApplicationImplementation
{
    HeightMapApplicationImplementation();

    void loadPrefs(Preferences &);
    void savePrefs(const Preferences &);

    ~HeightMapApplicationImplementation();

    HeightMapLogic *logic;

    const QString IniFilename;

private:
    DISABLE_COPY(HeightMapApplicationImplementation)
    DISABLE_MOVE(HeightMapApplicationImplementation)
};


HeightMapApplicationImplementation::HeightMapApplicationImplementation()
    : logic(nullptr),
      IniFilename(QApplication::applicationDirPath() + "/hmcfg.ini")
{
    ExtrapolationFactory *xFactorySimple = new SimpleExtrapolationFactory;
    extrapolations.insert(xFactorySimple->name(), xFactorySimple);

    ExtrapolationFactory *xFactorySlope = new SlopeExtrapolationFactory;
    extrapolations.insert(xFactorySlope->name(), xFactorySlope);

    ExtrapolationFactory *xFactoryBaseLevel = new BaseLevelExtrapolationFactory;
    extrapolations.insert(xFactoryBaseLevel->name(), xFactoryBaseLevel);

    ExtrapolationFactory *xFactoryFixedRadius = new FixedRadiusExtrapolationFactory;
    extrapolations.insert(xFactoryFixedRadius->name(), xFactoryFixedRadius);


}

void HeightMapApplicationImplementation::loadPrefs(Preferences &prefs)
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

void HeightMapApplicationImplementation::savePrefs(const Preferences &prefs)
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

HeightMapApplicationImplementation::~HeightMapApplicationImplementation() { }


HeightMapApplication::HeightMapApplication(int &argc, char **argv)
    : QApplication(argc, argv),
      m(new HeightMapApplicationImplementation)
{
    m->logic = new HeightMapLogic(this);

    Preferences prefs;
    m->loadPrefs(prefs);
    m->logic->setPreferences(prefs);
}


HeightMapLogic *HeightMapApplication::logic() const
{ return m->logic; }


HeightMapApplication::~HeightMapApplication()
{
    m->savePrefs(m->logic->preferences());
    delete m;
}


} // namespace HeightMap
