cmake_minimum_required(VERSION 3.10...3.21)

project(
  tev
  VERSION 0.1
  LANGUAGES CXX)

option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_TESTS "Build test" OFF)

### Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with "
                        "CMakeLists.txt file). Please make a build subdirectory. Feel free to "
                        "remove CMakeCache.txt and CMakeFiles.")
endif()

## Setup dependencies using CPM
set(CPM_SOURCE_CACHE "${PROJECT_SOURCE_DIR}/external")
include("cmake/getCPM.cmake")

if(BUILD_TESTS)
  CPMAddPackage(
    NAME Catch2
    GITHUB_REPOSITORY catchorg/Catch2
    GIT_TAG v2.5.0
  )
endif()

# use -std=c++xx instead of -std=g++xx
set(CMAKE_CXX_EXTENSIONS OFF)
# support folders in IDE's
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (APPLE)
  set(cxxlib libc++)
  set(force_compiler "compiler=apple-clang")
else ()
  set(cxxlib libstdc++11)
  set(force_compiler "")
endif()

set(CMAKE_CXX_STANDARD 11)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_subdirectory(src)

if(BUILD_EXAMPLES)
  add_subdirectory(examples/headerOnly)
  add_subdirectory(examples/echoHeader)
  add_subdirectory(examples/echoNulTer)
  add_subdirectory(examples/timeout)
  add_subdirectory(examples/builder)
  add_subdirectory(examples/httpClient)
endif()

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()