include(FetchContent)

FetchContent_Declare(fmtlib
  URL https://github.com/fmtlib/fmt/archive/refs/tags/12.1.0.tar.gz
  EXCLUDE_FROM_ALL
  SYSTEM
  OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(fmtlib)
