include(FetchContent)

FetchContent_Declare(sbe-code-gen
  GIT_REPOSITORY https://github.com/ksergey/sbe-code-gen.git
  GIT_TAG master
  GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(sbe-code-gen)
