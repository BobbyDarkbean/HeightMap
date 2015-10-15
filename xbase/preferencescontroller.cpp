#include "preferences.h"
#include "preferencescontroller.h"


namespace HeightMap {


PreferencesController::PreferencesController(QObject *parent)
    : m_prefs(nullptr) { }


Preferences *PreferencesController::preferences() const
{ return m_prefs; }

void PreferencesController::setPreferences(Preferences *prefs)
{ m_prefs = prefs; }


PreferencesController::~PreferencesController() { }


void PreferencesController::setLandscapeWidth(int landscapeWidth)
{ m_prefs->setLandscapeWidth(landscapeWidth); }

void PreferencesController::setLandscapeHeight(int landscapeHeight)
{ m_prefs->setLandscapeHeight(landscapeHeight); }

void PreferencesController::setPeakCount(int peakCount)
{ m_prefs->setPeakCount(static_cast<unsigned int>(peakCount)); }

void PreferencesController::setMinPeak(int minPeak)
{ m_prefs->setMinPeak(minPeak); }

void PreferencesController::setMaxPeak(int maxPeak)
{ m_prefs->setMaxPeak(maxPeak); }

void PreferencesController::setExtrapolatorName(QString extrapolatorName)
{ m_prefs->setExtrapolatorName(extrapolatorName); }

void PreferencesController::setMinContouringLevel(int minContouringLevel)
{ m_prefs->setMinContouringLevel(minContouringLevel); }

void PreferencesController::setMaxContouringLevel(int maxContouringLevel)
{ m_prefs->setMaxContouringLevel(maxContouringLevel); }

void PreferencesController::setContouringStep(int contouringStep)
{ m_prefs->setContouringStep(contouringStep); }

void PreferencesController::setImageFactor(int imageFactor)
{ m_prefs->setImageFactor(imageFactor); }


} // namespace HeightMap
