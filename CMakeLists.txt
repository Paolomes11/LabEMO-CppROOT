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
add_executable(test.t tests/test_file.cpp ${SOURCES})

# Definisci il test associato a test.t se i test sono abilitati
if(BUILD_TESTING)
      add_test(NAME test.t COMMAND test.t)
endif()


# Scripts per settaggio impostazioni e avvio programma

# find ROOT
find_package(ROOT REQUIRED)

# Imposta le directory include e link per ROOT
include_directories(${ROOT_INCLUDE_DIRS})
link_directories(${ROOT_LIBRARY_DIRS})

# Crea una cartella 'myRoot' per i file generati
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/root_files)

# Genera il file root_commands.C dinamicamente
configure_file(${CMAKE_SOURCE_DIR}/commands/configure_root_template.C.in
      ${CMAKE_BINARY_DIR}/commands/configure_root.C)

add_custom_target(configure_root
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/root_files
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/src/*.hpp ${CMAKE_SOURCE_DIR}/root_files
      COMMENT "Muovendo i file cpp e hpp generati in root_files"
)

# Comando personalizzato per eseguire ROOT
add_custom_command(TARGET configure_root POST_BUILD
      COMMAND ${ROOT_EXECUTABLE} -l -q ${CMAKE_BINARY_DIR}/commands/configure_root.C
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/root_files
      COMMENT "Eseguendo comandi ROOT"
)

# Comando per avviare il programma automaticamente
configure_file(${CMAKE_SOURCE_DIR}/commands/program_run_template.C.in
      ${CMAKE_BINARY_DIR}/commands/program_run.C)

add_custom_target(program_run
      COMMAND ${ROOT_EXECUTABLE} -l -q ${CMAKE_BINARY_DIR}/commands/program_run.C
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/
      COMMENT "Eseguendo comandi per il programma"
)

# Test per file.root
configure_file(${CMAKE_SOURCE_DIR}/commands/test_histogram_template.C.in
      ${CMAKE_BINARY_DIR}/commands/test_histogram.C)

add_custom_target(test_histogram
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/histograms.root ${CMAKE_SOURCE_DIR}/tests
      COMMENT "Muovendo l'istogramma in tests"
)

add_custom_command(TARGET test_histogram POST_BUILD
      COMMAND ${ROOT_EXECUTABLE} -l -q ${CMAKE_BINARY_DIR}/commands/test_histogram.C
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/
      COMMENT "Eseguendo il test per l'istorogramma"
)

