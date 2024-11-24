## Some useful commands but implemented in CMake
- Compiling a cpp with basics warnings
    - g++ ParticleType.cpp ResonanceType.cpp Particle.cpp main_test1.cpp -Wall -Wextra -o test1
- Build with Cmake in Debug mode 
    - cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug
- Configure the .iso file for root
    - gROOT->LoadMacro(“MyClass.cpp+”) // eseguire da root per creare file.so
- If you are working in VSCode, add this line in the c_cpp_properties.json (ctrl-shift-p) to let VSCode read all the needed includes
    - "includePath": ["${workspaceFolder}/**","/home/user/root/include"]

## Some rules to follow to change the program
- All the files must be organized in folders.
- If possible configure all with CMake.
- Is possible to create commands with CMake in order to execute long commands with a click.
- Try to comment in **english**.
- If you want to control something later **write** it down in the next paragraph and **use** a TO comment in the line of code and if solved **remove** it from here.
- If possible try to test every function, also use adress sanitizer or Valgrind.
- Try to write the code in an efficient way, this is particularly true if the function is used a lot of times (use *branchless* or *parallelism*).
- The .gitignore **exists**.

## Some things to note or check in the program
1. All the .C file were compacted but, if needed, we can decompact them; (**anyone in need**)
2. Check comments;

# Everything related to the histograms
## Numbers related to the single histogram and their names
0. histo_particles,
1. histo_azimutal,
2. histo_polar,
3. histo_impulse,
4. histo_transverse_impulse,
5. histo_energy,
6. histo_invmass,
7. histo_invmass_disc,
8. histo_invmass_conc,
9. histo_invmass_Pi_K_disc,
10. histo_invmass_Pi_K_conc,
11. histo_invmass_Ks_prod.

## Number of Entries and all intervals with a probability of 99.6%
- histo_particles
    - Mean = 1E7
- histo_azimutal
    - Mean = 1E7
- histo_polar
    - Mean = 1E7
- histo_impulse
    - Mean = 1E7
- histo_transverse_impulse
    - Mean = 1E7
- histo_energy
    - Mean = 1E7
- histo_invmass
    - Mean = 5.1513E8
    - Interval = [5.102-5.200]E8
- histo_invmass_disc
    - Mean = 2.526E8
    - Interval = [2.502-2.550]E8
- histo_invmass_conc
    - Mean = 2.526E8
    - Interval = [2.502-2.550]E8
- histo_invmass_Pi_K_disc
    - Mean = 4.400E7
    - Interval = [4.357-4.441]E7
- histo_invmass_Pi_K_conc
    - Mean = 4.400E7
    - Interval = [4.357-4.441]E7
- histo_invmass_Ks_prod
    - Mean = 10E5
    - Interval = [99051-100949]

# Results of the tests
The tests for the histograms have all been set with a percentage of 99% but, because of the mere number of tests related to probability, it is still rather common to see one or maximum two of the 31 tests fail.