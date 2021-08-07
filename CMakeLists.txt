# -----------------------------------------------------------------------------
# Project Setup

# NOTE: need CMake 3.9+ to specify project description:
#   https://cmake.org/cmake/help/latest/release/3.9.html?highlight=project%20description#commands
cmake_minimum_required(VERSION 3.9)
project(CalvinsUniversalHasher
    VERSION 1.0
    DESCRIPTION "A library for almost-universal hashing"
    LANGUAGES CXX)

# -----------------------------------------------------------------------------
# Library

add_library(caluhash INTERFACE src/include/caluhash.hpp)
target_include_directories(caluhash INTERFACE src/include)
target_compile_features(caluhash INTERFACE cxx_std_11) # need <random>

# -----------------------------------------------------------------------------
# Testing

enable_testing()

add_executable(caluhash_test test/main.cpp)
target_link_libraries(caluhash_test PRIVATE caluhash)
add_test(NAME caluhash_test COMMAND caluhash_test)

add_executable(caluhash_example test/example.cpp)
target_link_libraries(caluhash_example PRIVATE caluhash)
add_test(NAME caluhash_example COMMAND caluhash_example)

# -----------------------------------------------------------------------------
# Install  (I feel that this should be automatic given just the list of
# targets, but CMake requires us to explicitly list the things we want
# installed.)

install(TARGETS caluhash
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Absolute idiocy.  Why is there no canonical, out-of-the-box way to declare
# a public include directory?
install(
    FILES include/caluhash.h
    DESTINATION include)
