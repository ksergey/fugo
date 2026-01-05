function(sbe_make_codec TARGET)
  set(options)
  set(oneValueArgs SCHEMA OUTPUT PACKAGE)
  set(multiValueArgs)

  cmake_parse_arguments(PARSED "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  find_package(Java COMPONENTS Runtime REQUIRED)

  set(JAR "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/sbe-all-1.37.0.jar")

  add_custom_command(
    OUTPUT ${PARSED_OUTPUT}/${PARSED_PACKAGE}/MessageHeader.h
    DEPENDS ${PARSED_SCHEMA}
    COMMAND ${CMAKE_COMMAND} -E rm -rf "${PARSED_OUTPUT}"
    COMMAND ${Java_JAVA_EXECUTABLE} -Dsbe.output.dir="${PARSED_OUTPUT}" -Dsbe.target.namespace="${PARSED_PACKAGE}" -Dsbe.target.language=CPP -jar "${JAR}" "${PARSED_SCHEMA}"
    COMMENT "Generating schema (${PARSED_SCHEMA})"
  )

  add_library(${TARGET} INTERFACE EXCLUDE_FROM_ALL)
  target_compile_features(${TARGET} INTERFACE cxx_std_20)
  target_sources(${TARGET} INTERFACE "${PARSED_OUTPUT}/${PARSED_PACKAGE}/MessageHeader.h")
  target_include_directories(${TARGET} INTERFACE "${PARSED_OUTPUT}")
endfunction()

