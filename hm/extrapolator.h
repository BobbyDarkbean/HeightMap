#ifndef _Extrapolator_h_
#define _Extrapolator_h_


#include "hm_shared.h"


namespace HeightMap {


class HM_SHARED Extrapolator
{
public:
    Extrapolator();

    double baseLevel() const;
    void setBaseLevel(double);

    virtual double radius(double peak) const = 0;
    virtual void modify(double &currentValue,
                        double peak,
                        double vectorLength) = 0;

    virtual ~Extrapolator() = 0;

private:
    double m_baseLevel;
};


inline double Extrapolator::baseLevel() const
{ return m_baseLevel; }


class HM_SHARED SimpleExtrapolator
        : public Extrapolator
{
public:
    SimpleExtrapolator();

    double radius(double peak) const;
    void modify(double &currentValue,
                double peak,
                double vectorLength);

    ~SimpleExtrapolator();
};


class HM_SHARED BaseLevelExtrapolator
        : public Extrapolator
{
public:
    BaseLevelExtrapolator();

    double radius(double peak) const;
    void modify(double &currentValue,
                double peak,
                double vectorLength);

    ~BaseLevelExtrapolator();
};


class HM_SHARED SlopeExtrapolator
        : public Extrapolator
{
public:
    explicit SlopeExtrapolator(double slopeRatio = 1.0);

    double slopeRatio() const;
    void setSlopeRatio(double);

    double radius(double peak) const;
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

    double radius(double peak) const;
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
