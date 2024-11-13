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

# Add the directory 'src' and 'third_party' to the include directories

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/third_party)

# Define the source files in a variable
set(SOURCES
      src/Particle.cpp
      src/ParticleType.cpp
      src/ResonanceType.cpp
)

# find ROOT
find_package(ROOT REQUIRED)

# Set the ROOT include and library directories
include_directories(${ROOT_INCLUDE_DIRS})
link_directories(${ROOT_LIBRARY_DIRS})

# Create a 'root_files' directory and others for generated files
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/root_files)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/generated_files)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/generated_files/png_files)

# Test executables
add_executable(test_particles.t tests/test_file.cpp ${SOURCES})

add_executable(test_histo.t tests/HistoTest.cpp)
target_link_libraries(test_histo.t PRIVATE ${ROOT_LIBRARIES})
target_compile_options(test_histo.t PRIVATE -Wno-sign-conversion -Wno-conversion -Wno-float-conversion -Wno-extra-semi -Wno-vla) # For root warnings

if(BUILD_TESTING)
      add_test(NAME test_particles.t COMMAND test_particles.t)
      add_test(NAME test_histo.t COMMAND test_histo.t)

      # Configure AddressSanitizer for test_particles.t
      set_tests_properties(test_particles.t PROPERTIES
          COMPILE_FLAGS "-D_GLIBCXX_ASSERTIONS -fsanitize=address,undefined -fno-omit-frame-pointer"
          LINK_FLAGS "-fsanitize=address,undefined -fno-omit-frame-pointer")

      # Configure Valgrind for test_histo.t
      set(CTEST_MEMORYCHECK_COMMAND "valgrind")
      set(CTEST_MEMORYCHECK_OPTIONS "--leak-check=full --track-origins=yes")

      set_tests_properties(test_histo.t PROPERTIES
          MEMORYCHECK_COMMAND ${CTEST_MEMORYCHECK_COMMAND}
          MEMORYCHECK_COMMAND_OPTIONS ${CTEST_MEMORYCHECK_OPTIONS})
endif()

# Scripts for settings and starting the program
# Generate the file root_commands.C dynamically
configure_file(${CMAKE_SOURCE_DIR}/commands/configure_root_template.C.in
      ${CMAKE_BINARY_DIR}/commands/configure_root.C)

add_custom_target(configure_root
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/root_files
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/src/*.hpp ${CMAKE_SOURCE_DIR}/root_files
      COMMENT "Muovendo i file cpp e hpp generati in root_files"
)

# Personalized command to execute root
add_custom_command(TARGET configure_root POST_BUILD
      COMMAND ${ROOT_EXECUTABLE} -l -q ${CMAKE_BINARY_DIR}/commands/configure_root.C
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/root_files
      COMMENT "Eseguendo comandi ROOT"
)

# Command to execute the program automatically
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

# Command to start the analyzer
configure_file(${CMAKE_SOURCE_DIR}/commands/analyzer_run_template.C.in
      ${CMAKE_BINARY_DIR}/commands/analyzer_run.C)

add_custom_target(analyzer_run
      COMMAND ${ROOT_EXECUTABLE} -l -q ${CMAKE_BINARY_DIR}/commands/analyzer_run.C
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/
      COMMENT "Analizzando il file.root"
)

# Test for file.root
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
