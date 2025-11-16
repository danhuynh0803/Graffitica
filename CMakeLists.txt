cmake_policy(SET CMP0048 NEW)

set(PROJECT_NAME graffitica)

project(${PROJECT_NAME}
    #VERSION 0.1
    #DESCRIPTION "Software Rasterizer"
    #LANGUAGES CXX
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
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

    foreach(SOURCE_FILE ${SOURCES})
        # Extract the relative path from the source directory
        string(REPLACE "${CMAKE_SOURCE_DIR}/src/" "" RELATIVE_PATH "${SOURCE_FILE}")

        # Extract the directory part of the relative path
        get_filename_component(DIR_NAME "${RELATIVE_PATH}" DIRECTORY)

        # Replace slashes with backslashes for Visual Studio folder structure
        string(REPLACE "/" "\\" VS_FOLDER "${DIR_NAME}")

        # Group the file into the corresponding Visual Studio folder
        source_group("${VS_FOLDER}" FILES "${SOURCE_FILE}")
    endforeach()

add_executable(${PROJECT_NAME}
    ${SOURCES}
)

# Allow for VS cpu instrumentation
if (MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/PROFILE")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${SDL3_Headers}"
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    SDL3::SDL3
)

# OpenMP
find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
    target_compile_options(${PROJECT_NAME} PRIVATE
        ${OpenMP_CXX_FLAGS} # Or OpenMP_C_FLAGS, OpenMP_Fortran_FLAGS
        /W4     # level 4 warnings
        #/WX    # warnings as errors
    )
endif()


add_subdirectory(external)
add_subdirectory(test)
