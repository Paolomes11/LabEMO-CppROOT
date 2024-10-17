#include "Particle.hpp"

int main()
{
    char name1 = 'N';
    char name2 = 'T';
    ParticleType *p[2];
    p[0] = new ParticleType(&name1, 14.5, +1);
    p[1] = new ResonanceType(&name2, 17.5, -1, 24.3);

    for (int i = 0; i < 2; ++i)
    {
        p[i]->Print();
    }

    Particle::AddParticleType("pallo", 18.34, +1, 22.);
    Particle::AddParticleType("pello", 18.34, +1, 22.);
    Particle::AddParticleType("pillo", 1.34, +2, 2.);
    Particle::AddParticleType("pollo", 1.34, +2, 2.);
    Particle::AddParticleType("pullo", 1.34, +2, 2.);
    Particle::AddParticleType("pullo", 1.34, +2, 2.);
    Particle::AddParticleType("pullo", 1.34, +2, 2.);
    Particle::AddParticleType("pullo", 1.34, +2, 2.);
    Particle::AddParticleType("pullo", 1.34, +2, 2.);
    Particle::AddParticleType("pullo", 1.34, +2, 2.);
    Particle::AddParticleType("pullo", 1.34, +2, 2.);

    return 0;
}