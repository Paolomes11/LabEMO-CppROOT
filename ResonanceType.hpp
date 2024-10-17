#ifndef RESONANCETYPE_HPP
#define RESONANCETYPE_HPP
#include "ParticleType.hpp"

class ResonanceType : public virtual ParticleType
{
    public:
    ResonanceType(const char &Name, double Mass, int Charge, double Width); //ctor
    
    //getter
    double GetWidth() const;
    
    //print attributes override
    void Print() const override;

    private:
    double const fWidth;
};

#endif