cmake_minimum_required(VERSION 3.16)
project(root_project VERSION 0.1.0)

# abilita il supporto per i test, tra cui l'opzione BUILD_TESTING usata sotto
include(CTest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# richiedi l'uso di C++17, senza estensioni non-standard offerte dal compilatore usato 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# abilita warning
string(APPEND CMAKE_CXX_FLAGS
      " -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion"
      " -Wshadow -Wimplicit-fallthrough -Wextra-semi -Wold-style-cast")

# abilita asserzioni di debug (in gcc), l'address sanitizer e l'undefined-behaviour sanitizer in debug mode
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -D_GLIBCXX_ASSERTIONS -fsanitize=address,undefined -fno-omit-frame-pointer")
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address,undefined -fno-omit-frame-pointer")


add_executable(test1 main_test1.cpp Particle.cpp ParticleType.cpp ResonanceType.cpp)

add_executable(test.t test_file.cpp Particle.cpp ParticleType.cpp ResonanceType.cpp)
add_test(NAME test.t COMMAND test.t)

# se il testing e' abilitato...
#   per disabilitare il testing, passare -DBUILD_TESTING=OFF a cmake durante la fase di configurazione
# if (BUILD_TESTING)

#   # aggiungi l'eseguibile NAME.t
#   add_executable(probability.t ants_simulator/probability.test.cpp)
#   add_executable(mapweight.t ants_simulator/mapweight.test.cpp ants_simulator/mapweight.cpp)
#   # aggiungi l'eseguibile NAME.t alla lista dei test
#   add_test(NAME probability.t COMMAND probability.t)
#   add_test(NAME mapweight.t COMMAND mapweight.t)

#   add_executable(maptranslate.t test_maptranslate/maptranslate.test.cpp test_maptranslate/maptranslate.cpp)
#   target_link_libraries(maptranslate.t PRIVATE sfml-graphics)
#   add_test(NAME maptranslate.t COMMAND maptranslate.t)

# endif()