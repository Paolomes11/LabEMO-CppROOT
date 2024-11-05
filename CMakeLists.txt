cmake_minimum_required(VERSION 3.16)
project(root_project VERSION 0.1.0)

# abilita il supporto per i test, tra cui l'opzione BUILD_TESTING usata sotto
include(CTest)

# Abilita la variabile BUILD_TESTING, per compilare o meno i test
if(BUILD_TESTING)
      enable_testing()
endif()

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


# Aggiungi la directory 'src' e 'third_party' alle directory di inclusione
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/third_party)

# Definisci i file sorgente comuni in una variabile
set(SOURCES
      src/Particle.cpp
      src/ParticleType.cpp
      src/ResonanceType.cpp
)

# Eseguibili di test
add_executable(test1 tests/main_test1.cpp ${SOURCES})
add_executable(test.t tests/test_file.cpp ${SOURCES})
# add_executable(testRand src/test.cpp ${SOURCES})

# Definisci il test associato a test.t se i test sono abilitati
if(BUILD_TESTING)
      add_test(NAME test.t COMMAND test.t)
endif()

# # Trova il pacchetto ROOT per il file random_generation.C
# find_package(ROOT REQUIRED)

# include_directories(${ROOT_INCLUDE_DIRS})
# link_directories(${ROOT_LIBRARY_DIR})

# # Definisci l'output directory per i file .so
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/myRoot)

# # Crea le librerie condivise (.so)
# add_library(Particle SHARED src/Particle.cpp)
# add_library(ParticleType SHARED src/ParticleType.cpp)
# add_library(ResonanceType SHARED src/ResonanceType.cpp)

# # Linka ROOT solo a random_generation.C
# add_executable(random_generation myRoot/random_generation.C)
# target_link_libraries(random_generation ${ROOT_LIBRARIES})
