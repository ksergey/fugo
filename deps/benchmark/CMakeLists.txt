include(FetchContent)

FetchContent_Declare(benchmark
  URL https://github.com/google/benchmark/archive/refs/tags/v1.8.4.tar.gz
  EXCLUDE_FROM_ALL
  SYSTEM
  OVERRIDE_FIND_PACKAGE
)

set(BENCHMARK_ENABLE_GTEST_TESTS FALSE)
set(BENCHMARK_ENABLE_ASSEMBLY_TESTS FALSE)
FetchContent_MakeAvailable(benchmark)
