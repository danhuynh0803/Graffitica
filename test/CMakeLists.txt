project(tests
    DESCRIPTION "testing framework"
)

cmake_minimum_required(VERSION 3.16)

# Set all libs to be static
option(BUILD_SHARED_LIBS OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if (EXISTS "${LOC_PATH}")
  message(FATAL_ERROR "build must be out of source")
endif()

# TODO clean up and split cmakelists between modules later
#file(GLOB_RECURSE HEADERS "*.h")
#file(GLOB_RECURSE SOURCE "*.cpp")

add_executable(tests
    math/tests-vec.cpp
)

target_include_directories(tests PUBLIC
    ./
    ../src/
    Catch2::Catch2WithMain
)

target_link_libraries(tests PUBLIC
    Catch2::Catch2WithMain
)

add_subdirectory(external/Catch2)
