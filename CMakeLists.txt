cmake_minimum_required(VERSION 3.24)

project(Flinn CXX)

option(FLINN_SANITIZER "Build with sanitizer" OFF)

include(cmake/FlinnUtils.cmake)

if (NOT TARGET doctest::doctest_with_main)
  add_subdirectory(deps/doctest)
endif()
if (NOT TARGET benchmark::benchmark_main)
  add_subdirectory(deps/benchmark)
endif()
if (NOT TARGET cxxopts::cxxopts)
  add_subdirectory(deps/cxxopts)
endif()
if (NOT TARGET turboq::turboq)
  add_subdirectory(deps/turboq)
endif()

enable_testing()

add_subdirectory(flinn_core)
add_subdirectory(flinn_logger)
