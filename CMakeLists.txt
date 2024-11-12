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

# Aggiungi il flag -fopenmp per GCC
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

# abilita warning
string(APPEND CMAKE_CXX_FLAGS
      " -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion"
      " -Wshadow -Wimplicit-fallthrough -Wextra-semi") # -Wold-style-cast")

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

# find ROOT
find_package(ROOT REQUIRED)

# Imposta le directory include e link per ROOT
include_directories(${ROOT_INCLUDE_DIRS})
link_directories(${ROOT_LIBRARY_DIRS})

# Crea una cartella 'myRoot' per i file generati
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/root_files)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/generated_files)

# Eseguibili di test
add_executable(test_particles.t tests/test_file.cpp ${SOURCES})

add_executable(test_histo.t tests/HistoTest.cpp)
target_link_libraries(test_histo.t PRIVATE ${ROOT_LIBRARIES})

# Definisci il test associato a test.t se i test sono abilitati
if(BUILD_TESTING)
      add_test(NAME test_particles.t COMMAND test_particles.t)
      add_test(NAME test_histo.t COMMAND test_histo.t)
endif()


# Scripts per settaggio impostazioni e avvio programma

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
      COMMENT "Generando il file.root"
)

add_custom_command(TARGET program_run POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/histograms.root ${CMAKE_SOURCE_DIR}/generated_files
      COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_SOURCE_DIR}/histograms.root
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/generated_files
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/generated_files/histograms.root ${CMAKE_BINARY_DIR}/generated_files # Copia i file generati in build
      COMMENT "Muovendo l'istogramma in tests e eliminandolo dal main"
)

# Comando per avviare l'analisi automaticamente
configure_file(${CMAKE_SOURCE_DIR}/commands/analyzer_run_template.C.in
      ${CMAKE_BINARY_DIR}/commands/analyzer_run.C)

add_custom_target(analyzer_run
      COMMAND ${ROOT_EXECUTABLE} -l -q ${CMAKE_BINARY_DIR}/commands/analyzer_run.C
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/
      COMMENT "Analizzando il file.root"
)

# Test per file.root
configure_file(${CMAKE_SOURCE_DIR}/commands/test_graph_result_template.C.in
      ${CMAKE_BINARY_DIR}/commands/test_graph_result.C)

add_custom_target(test_graph_result
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/generated_files/histograms.root ${CMAKE_SOURCE_DIR}/tests
      COMMENT "Muovendo l'istogramma in tests"
)

add_custom_command(TARGET test_graph_result POST_BUILD
      COMMAND ${ROOT_EXECUTABLE} -l -q ${CMAKE_BINARY_DIR}/commands/test_graph_result.C
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests
      COMMENT "Eseguendo il test per l'istorogramma"
)

add_custom_command(TARGET test_graph_result POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_SOURCE_DIR}/tests/histograms.root
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/tests/Histograms.gif ${CMAKE_SOURCE_DIR}/generated_files
      COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_SOURCE_DIR}/tests/Histograms.gif
      COMMENT "Rimuovendo il file.root da tests/ e muovendo il file.gif in generated_files"
)
