#include <QSettings>
#include "preferences.h"
#include "preferencescontroller.h"
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
    ~HeightMapApplicationImplementation();

    HeightMapLogic *logic;

    Preferences prefs;
    PreferencesController *ctrl;
    QMap<QString, ExtrapolationFactory *> extrapolations;

    const QString IniFilename;

private:
    DISABLE_COPY(HeightMapApplicationImplementation)
    DISABLE_MOVE(HeightMapApplicationImplementation)
};


HeightMapApplicationImplementation::HeightMapApplicationImplementation()
    : logic(nullptr),
      prefs(),
      ctrl(nullptr),
      extrapolations(),
      IniFilename(QApplication::applicationDirPath() + "/hmcfg.ini")
{
    ExtrapolationFactory *xFactorySimple = new SimpleExtrapolationFactory;
    ExtrapolationFactory *xFactorySlope = new SlopeExtrapolationFactory;
    ExtrapolationFactory *xFactoryBaseLevel = new BaseLevelExtrapolationFactory;
    ExtrapolationFactory *xFactoryFixedRadius = new FixedRadiusExtrapolationFactory;
    extrapolations.insert(xFactorySimple->name(), xFactorySimple);
    extrapolations.insert(xFactorySlope->name(), xFactorySlope);
    extrapolations.insert(xFactoryBaseLevel->name(), xFactoryBaseLevel);
    extrapolations.insert(xFactoryFixedRadius->name(), xFactoryFixedRadius);

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


HeightMapApplication::HeightMapApplication(int &argc, char **argv)
    : QApplication(argc, argv),
      m(new HeightMapApplicationImplementation)
{
    m->logic = new HeightMapLogic(this);

    m->ctrl = new PreferencesController(this);
    m->ctrl->setPreferences(&m->prefs);
}


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


HeightMapLogic *HeightMapApplication::logic() const
{ return m->logic; }


QStringList HeightMapApplication::extrapolatorKeys() const
{
    return m->extrapolations.keys();
}

AbstractExtrapolationWidget *HeightMapApplication::createExtrapolationWidget(const QString &name) const
{
    if (ExtrapolationFactory *f = m->extrapolations.value(name, nullptr)) {
        return f->createWidget();
    }

    return nullptr;
}

QString HeightMapApplication::extrapolationDescription(const QString &name) const
{
    if (ExtrapolationFactory *f = m->extrapolations.value(name, nullptr)) {
        return f->description();
    }

    return QString();
}

Extrapolator *HeightMapApplication::currentExtrapolator() const
{
    QString currentExtrapolatorName = preferences().extrapolatorName();
    if (ExtrapolationFactory *f = m->extrapolations.value(currentExtrapolatorName, nullptr)) {
        return f->extrapolator();
    }

    return nullptr;
}


HeightMapApplication::~HeightMapApplication()
{
    delete m;
}


} // namespace HeightMap
