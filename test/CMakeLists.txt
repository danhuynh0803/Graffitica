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

#file(GLOB_RECURSE SOURCES "*.cpp" "*.h")
#
#    foreach(SOURCE_FILE ${SOURCES})
#        # Extract the relative path from the source directory
#        string(REPLACE "${CMAKE_SOURCE_DIR}/src/" "" RELATIVE_PATH "${SOURCE_FILE}")
#
#        # Extract the directory part of the relative path
#        get_filename_component(DIR_NAME "${RELATIVE_PATH}" DIRECTORY)
#
#        # Replace slashes with backslashes for Visual Studio folder structure
#        string(REPLACE "/" "\\" VS_FOLDER "${DIR_NAME}")
#
#        # Group the file into the corresponding Visual Studio folder
#        source_group("${VS_FOLDER}" FILES "${SOURCE_FILE}")
#    endforeach()


add_executable(tests
    math/test_vec.cpp
    math/test_matrix.cpp
)

target_include_directories(tests PUBLIC
    ./
    ../src/
    Catch2::Catch2WithMain
)

target_link_libraries(tests PUBLIC
    Catch2::Catch2WithMain
)
