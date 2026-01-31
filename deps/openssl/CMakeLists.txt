include(FetchContent)

FetchContent_Declare(openssl
  URL https://github.com/openssl/openssl/releases/download/openssl-3.6.0/openssl-3.6.0.tar.gz
  EXCLUDE_FROM_ALL
  SYSTEM
  OVERRIDE_FIND_PACKAGE
)

FetchContent_GetProperties(OpenSSL)
if (NOT openssl_POPULATED)
  FetchContent_MakeAvailable(OpenSSL)
endif()

execute_process(
  COMMAND ${openssl_SOURCE_DIR}/config --release no-tests no-shared
  WORKING_DIRECTORY ${openssl_BINARY_DIR}
  COMMAND_ERROR_IS_FATAL ANY
)

add_custom_command(
  OUTPUT
    ${openssl_BINARY_DIR}/libcrypto.a
    ${openssl_BINARY_DIR}/libssl.a
  COMMAND
    make build_libs
  WORKING_DIRECTORY ${openssl_BINARY_DIR}
  VERBATIM
)

add_custom_target(openssl-build
  DEPENDS
    ${openssl_BINARY_DIR}/libcrypto.a
    ${openssl_BINARY_DIR}/libssl.a
  COMMENT "Building OpenSSL"
)

find_package(Threads)
if (Threads_FOUND)
  list(APPEND openssl_link_dependencies Threads::Threads)
  list(APPEND openssl_dependencies ${CMAKE_THREAD_LIBS_INIT})
endif()
list(APPEND openssl_link_dependencies ${CMAKE_DL_LIBS})

set(OPENSSL_FOUND ON CACHE BOOL "Override FindOpenSSL variables" FORCE)
set(OPENSSL_INCLUDE_DIR ${openssl_SOURCE_DIR}/include ${openssl_BINARY_DIR}/include CACHE STRING "Override FindOpenSSL variables" FORCE)
set(OPENSSL_CRYPTO_LIBRARY ${openssl_BINARY_DIR}/libcrypto.a CACHE STRING "Override FindOpenSSL variables" FORCE)
set(OPENSSL_CRYPTO_LIBRARIES ${openssl_BINARY_DIR}/libcrypto.a ${openssl_dependencies} CACHE STRING "Override FindOpenSSL variables" FORCE)
set(OPENSSL_SSL_LIBRARY ${openssl_BINARY_DIR}/libssl.a CACHE STRING "Override FindOpenSSL variables" FORCE)
set(OPENSSL_SSL_LIBRARIES ${openssl_BINARY_DIR}/libssl.a ${openssl_dependencies} CACHE STRING "Override FindOpenSSL variables" FORCE)
set(OPENSSL_LIBRARIES ${openssl_BINARY_DIR}/libcrypto.a ${openssl_BINARY_DIR}/libssl.a ${openssl_dependencies} CACHE STRING "Override FindOpenSSL variables" FORCE)
set(OPENSSL_VERSION "1.1.1t" CACHE STRING "Override FindOpenSSL variables" FORCE)

add_library(OpenSSL::Crypto STATIC IMPORTED GLOBAL)
set_target_properties(OpenSSL::Crypto PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES C
  IMPORTED_LOCATION "${openssl_BINARY_DIR}/libcrypto.a"
  INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}"
  INTERFACE_LINK_LIBRARIES "${openssl_link_dependencies}"
)
add_dependencies(OpenSSL::Crypto openssl-build)

add_library(OpenSSL::SSL STATIC IMPORTED GLOBAL)
set_target_properties(OpenSSL::SSL PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES C
  IMPORTED_LOCATION "${openssl_BINARY_DIR}/libssl.a"
  INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}"
  INTERFACE_LINK_LIBRARIES "${openssl_link_dependencies};OpenSSL::Crypto"
)
add_dependencies(OpenSSL::SSL openssl-build)
