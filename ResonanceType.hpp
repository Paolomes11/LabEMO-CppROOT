#ifndef RESONANCETYPE_HPP
#define RESONANCETYPE_HPP
#include "ParticleType.hpp"

class ResonanceType : public ParticleType
{
    public:
    ResonanceType(char &fName, double fMass, int fCharge, double fWidth); //ctor
    virtual ~ResonanceType(); //dtor
    
    //getter
    double GetWidth() const;
    
    //print attributes override
    void Print() const override;

    private:
    double const fWidth_;
};

#endif