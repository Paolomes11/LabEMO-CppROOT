## Some useful commands but implemented in Cmake
- Compiling a cpp with basics warnings
    - g++ ParticleType.cpp ResonanceType.cpp Particle.cpp main_test1.cpp -Wall -Wextra -o test1
- Build with Cmake in Debug mode 
    - cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug
- Configure the .iso file for root
    - gROOT->LoadMacro(“MyClass.cpp+”) // eseguire da root per creare file.so

