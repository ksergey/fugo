cmake_minimum_required(VERSION 3.24)

project(Fugo CXX)

option(FUGO_SANITIZER "Build with sanitizer" OFF)

include(cmake/FugoUtils.cmake)

if (NOT TARGET doctest::doctest_with_main)
  add_subdirectory(deps/doctest)
endif()
if (NOT TARGET benchmark::benchmark_main)
  add_subdirectory(deps/benchmark)
endif()
if (NOT TARGET fmt::fmt-header-only)
  add_subdirectory(deps/fmtlib)
endif()
if (NOT TARGET nlohmann_json::nlohmann_json)
  add_subdirectory(deps/nlohmann_json)
endif()
if (NOT TARGET cxxopts::cxxopts)
  add_subdirectory(deps/cxxopts)
endif()
if (NOT TARGET CURL::libcurl)
  add_subdirectory(deps/curl)
endif()
if (NOT TARGET turboq::turboq)
  add_subdirectory(deps/turboq)
endif()

add_subdirectory(deps/simple-binary-encoding)

enable_testing()

add_subdirectory(libs)
add_subdirectory(examples)
