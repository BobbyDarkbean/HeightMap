#ifndef _Extrapolator_h_
#define _Extrapolator_h_


#include "hm_shared.h"


namespace HeightMap {


class HM_SHARED Extrapolator
{
public:
    virtual int radius(double peak) const = 0;
    virtual void modify(double &currentValue,
                        double peak,
                        double vectorLength) = 0;

    virtual ~Extrapolator() = 0;
};


class HM_SHARED SimpleExtrapolator
        : public Extrapolator
{
public:
    SimpleExtrapolator();

    int radius(double peak) const;
    void modify(double &currentValue,
                double peak,
                double vectorLength);

    ~SimpleExtrapolator();
};


class HM_SHARED BaseLevelExtrapolator
        : public Extrapolator
{
public:
    explicit BaseLevelExtrapolator(double baseLevel = 0.0);

    double baseLevel() const;
    void setBaseLevel(double);

    int radius(double peak) const;
    void modify(double &currentValue,
                double peak,
                double vectorLength);

    ~BaseLevelExtrapolator();

private:
    double m_baseLevel;
};


inline double BaseLevelExtrapolator::baseLevel() const
{ return m_baseLevel; }


class HM_SHARED SlopeExtrapolator
        : public Extrapolator
{
public:
    explicit SlopeExtrapolator(double slopeRatio = 1.0);

    double slopeRatio() const;
    void setSlopeRatio(double);

    int radius(double peak) const;
    void modify(double &currentValue,
                double peak,
                double vectorLength);

    ~SlopeExtrapolator();

private:
    double m_slopeRatio;
};


inline double SlopeExtrapolator::slopeRatio() const
{ return m_slopeRatio; }


class HM_SHARED FixedRadiusExtrapolator
        : public Extrapolator
{
public:
    explicit FixedRadiusExtrapolator(int fixedRadius = 16);

    int fixedRadius() const;
    void setFixedRadius(int);

    int radius(double peak) const;
    void modify(double &currentValue,
                double peak,
                double vectorLength);

    ~FixedRadiusExtrapolator();

private:
    double m_fixedRadius;
};


inline int FixedRadiusExtrapolator::fixedRadius() const
{ return m_fixedRadius; }


} // namespace HeightMap


#endif // _Extrapolator_h_
