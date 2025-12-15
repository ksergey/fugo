set(TargetName fugo_service)

add_library(${TargetName})
target_compile_features(${TargetName}
  PUBLIC cxx_std_23)
set_target_properties(${TargetName}
  PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF)
target_compile_options(${TargetName}
  PUBLIC -Wall -Wextra -Wattributes -Wpedantic -Wstrict-aliasing -Wcast-align -g)
target_include_directories(${TargetName}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${TargetName}
  PUBLIC fugo::core fmt::fmt)

add_library(fugo::service ALIAS ${TargetName})

file(GLOB_RECURSE Headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB_RECURSE Sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

FugoAddTestsFromSourceList(Sources
  PREFIX ${TargetName}
  COMPILE_OPTIONS -Wall -Wextra -g
  LIBS doctest::doctest_with_main ${TargetName})

FugoAddBenchmarksFromSourceList(Sources
  PREFIX ${TargetName}
  COMPILE_OPTIONS -Wall -Wextra -g
  LIBS benchmark::benchmark_main ${TargetName})

FugoExcludeTestsAndBenchmarksFromSourceList(Sources)

target_sources(${TargetName} PUBLIC ${Headers})
target_sources(${TargetName} PRIVATE ${Sources})
