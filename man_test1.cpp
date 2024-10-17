#include "ResonanceType.hpp"

int main() {
    const char name1 = 'N';
    ParticleType proton(name1, 14.5, 1);

    for(int i= 0; i < 2; ++i){
        proton.Print();
    }

    return 0;
    }
