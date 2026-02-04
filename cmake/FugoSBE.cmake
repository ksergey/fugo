include(CMakeParseArguments)

function(FugoSBEMakeCodec TARGET)
  set(options)
  set(oneValueArgs SCHEMA OUTPUT PACKAGE)
  set(multiValueArgs)

  cmake_parse_arguments(FUGO_PARSED "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  find_package(Java COMPONENTS Runtime REQUIRED)

  set(JAR "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/sbe-all-1.37.0.jar")

  add_custom_command(
    OUTPUT ${FUGO_PARSED_OUTPUT}/${FUGO_PARSED_PACKAGE}/MessageHeader.h
    DEPENDS ${FUGO_PARSED_SCHEMA}
    COMMAND ${CMAKE_COMMAND} -E rm -rf "${FUGO_PARSED_OUTPUT}"
    COMMAND ${Java_JAVA_EXECUTABLE} -Dsbe.output.dir="${FUGO_PARSED_OUTPUT}" -Dsbe.target.namespace="${FUGO_PARSED_PACKAGE}" -Dsbe.target.language=CPP -jar "${JAR}" "${FUGO_PARSED_SCHEMA}"
    COMMENT "Generating schema (${FUGO_PARSED_SCHEMA})"
  )

  add_library(${TARGET} INTERFACE EXCLUDE_FROM_ALL)
  target_compile_features(${TARGET} INTERFACE cxx_std_20)
  target_sources(${TARGET} INTERFACE "${FUGO_PARSED_OUTPUT}/${FUGO_PARSED_PACKAGE}/MessageHeader.h")
  target_include_directories(${TARGET} INTERFACE "${FUGO_PARSED_OUTPUT}")
endfunction()

