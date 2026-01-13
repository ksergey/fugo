set(TargetName FeedHandler)

sbe_make_codec(FeedHandler-messaging
  PACKAGE sbe_local
  SCHEMA ${CMAKE_CURRENT_SOURCE_DIR}/messaging.xml
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/messaging
)

add_executable(${TargetName})
target_compile_features(${TargetName}
  PRIVATE cxx_std_23)
set_target_properties(${TargetName}
  PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF)
target_compile_options(${TargetName}
  PRIVATE -Wall -Wextra -Wattributes -Wpedantic -Wstrict-aliasing -Wcast-align -g -fno-rtti)
target_compile_definitions(${TargetName}
  PRIVATE -DCXXOPTS_NO_RTTI)
target_link_libraries(${TargetName}
  PRIVATE cxxopts::cxxopts fmt::fmt-header-only fugo::service fugo::logger fugo::config)
target_link_libraries(${TargetName}
  PRIVATE FeedHandler-messaging)

target_link_libraries(${TargetName}
  PRIVATE CURL::libcurl)

file(GLOB_RECURSE Headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB_RECURSE Sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

FugoAddTestsFromSourceList(Sources
  PREFIX ${TargetName}
  COMPILE_OPTIONS -Wall -Wextra -g
  LIBS doctest::doctest_with_main)

FugoAddBenchmarksFromSourceList(Sources
  PREFIX ${TargetName}
  COMPILE_OPTIONS -Wall -Wextra -g
  LIBS benchmark::benchmark_main)

FugoExcludeTestsAndBenchmarksFromSourceList(Sources)

FugoRemoveMatchesFromList(Sources MATCHES dump.cpp)

target_sources(${TargetName} PUBLIC ${Headers})
target_sources(${TargetName} PRIVATE ${Sources})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config.json
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(TargetName dump)
add_executable(${TargetName} dump.cpp)
target_compile_features(${TargetName}
  PRIVATE cxx_std_23)
set_target_properties(${TargetName}
  PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF)
target_compile_options(${TargetName}
  PRIVATE -Wall -Wextra -Wattributes -Wpedantic -Wstrict-aliasing -Wcast-align -g -fno-rtti)
target_compile_definitions(${TargetName}
  PRIVATE -DCXXOPTS_NO_RTTI)
target_link_libraries(${TargetName}
  PRIVATE cxxopts::cxxopts fmt::fmt-header-only fugo::service)
target_link_libraries(${TargetName}
  PRIVATE FeedHandler-messaging)
