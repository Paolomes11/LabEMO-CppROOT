#ifndef RESONANCETYPE_HPP
#define RESONANCETYPE_HPP
#include "ParticleType.hpp"

class ResonanceType : public ParticleType
{
    public:
    ResonanceType(double fWidth); //ctor
    virtual ~ResonanceType(); //dtor
    
    //getter
    double GetWidth() const;
    
    //print attributes
    void Print() const override;

    private:
    double const fWidth_;
};

#endif