include(FetchContent)

FetchContent_Declare(turboq
  GIT_REPOSITORY https://github.com/ksergey/turboq.git
  GIT_TAG master
  GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(turboq)
