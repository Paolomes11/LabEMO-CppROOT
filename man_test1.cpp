#include "ParticleType.hpp"
#include "ResonanceType.hpp"

int main() {
    ParticleType * p[2];
    p[0] = new ParticleType("Name1", 14.5, +1);
    p[1] = new ResonanceType("Name2", 17.5, -1, 24.3);

    for(int i= 0; i < 2; ++i){
        p[i].Print();
    }

    return 0;
    }