cmake_minimum_required(VERSION 3.10)
project(pimpl_on_stack_library)

set(ENABLE_TESTS TRUE CACHE BOOL "Wether compile tests or not")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(pimpl_on_stack INTERFACE)
target_include_directories(pimpl_on_stack INTERFACE include)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)

if (${ENABLE_TESTS})
  if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.14/conan.cmake"
      "${CMAKE_BINARY_DIR}/conan.cmake")
  endif ()

  include(${CMAKE_BINARY_DIR}/conan.cmake)
  set(ENV{CONAN_USER_HOME} ${PROJECT_SOURCE_DIR})
  if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    set(CONAN_COMPILER gcc)
  else ()
    set(CONAN_COMPILER clang)
  endif ()
  string(SUBSTRING ${CMAKE_CXX_COMPILER_VERSION} 0 3 CONAN_COMPILER_VERSION)

  if (${CONAN_COMPILER} STREQUAL clang AND ${CONAN_COMPILER_VERSION} STREQUAL 9.0)
    set(CONAN_COMPILER_VERSION 9)
  endif ()

  conan_cmake_run(REQUIRES gtest/1.10.0
    BASIC_SETUP CMAKE_TARGETS
    BUILD missing
    SETTINGS compiler=${CONAN_COMPILER}
    compiler.version=${CONAN_COMPILER_VERSION}
    compiler.libcxx=libstdc++11
    build_type=Release
    ENV CC=${CMAKE_C_COMPILER}
    CXX=${CMAKE_CXX_COMPILER}
    )

  add_executable(pimpl_on_stack_tests tests/pimpl_on_stack_tests.cpp)
  target_link_libraries(pimpl_on_stack_tests
    PRIVATE
    pimpl_on_stack
    CONAN_PKG::gtest
    )
  enable_testing()
  add_test(NAME pimpl_on_stack_tests COMMAND pimpl_on_stack_tests)
endif ()
