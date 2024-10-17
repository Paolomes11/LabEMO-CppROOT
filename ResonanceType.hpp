#ifndef RESONANCETYPE_HPP
#define RESONANCETYPE_HPP
#include "ParticleType.hpp"

class ResonanceType : public ParticleType
{
    public:
    ResonanceType(double fWidth);
    double GetWidth() const;
    protected:

    private:
    double const fWidth_;
};

#endif