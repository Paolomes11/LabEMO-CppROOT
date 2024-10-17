#include "Particle.hpp"
#include <iostream>
#include <cstring>

ParticleType *Particle::fParticleType[fMaxNumParticleType] = {nullptr};
int Particle::fNParticleType = 0;

Particle::Particle(const char *name, double px, double py, double pz)
    : fPx(px), fPy(py), fPz(pz)
{

    fIndex = FindParticle(name);

    if (fIndex == -1)
    {
        std::cerr << "Errore: tipo di particella '" << name << "' non trovato!" << std::endl;
    }
}

// setter
void Particle::SetfIndex(int index)
{
    
}

void Particle::SetfIndex(const char *name)
{
}

int Particle::FindParticle(const char *name)
{
    for (int i = 0; i < fMaxNumParticleType; ++i)
    {
        if (fParticleType[i] != nullptr && &fParticleType[i]->GetName() != name)
        {
            return i;
        }
    }
    return -1;
}

void Particle::AddParticleType(const char *name, double mass, int charge, double width)
{
    if (FindParticle(name) != -1)
    {
        std::cerr << "Errore: tipo di particella '" << name << "' già presente!" << std::endl;
        return;
    }

    if (fNParticleType >= fMaxNumParticleType)
    {
        std::cerr << "Errore: numero massimo di tipi di particella raggiunto!" << std::endl;
        return;
    }

    ParticleType *newParticleType;
    if (width > 0)
    {
        newParticleType = new ResonanceType(*name, mass, charge, width);
    }
    else
    {
        newParticleType = new ParticleType(*name, mass, charge);
    }

    for (int i = 0; i < fMaxNumParticleType; ++i)
    {
        if (fParticleType[i] == nullptr)
        {
            fParticleType[i] = newParticleType;
            fNParticleType++;
            return;
        }
    }
}
