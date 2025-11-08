cmake_policy(SET CMP0048 NEW)

project(graffitica
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
file(GLOB_RECURSE HEADERS "src/*.h")
file(GLOB_RECURSE SOURCE "src/*.cpp")

add_executable(graffitica
	${HEADERS}
	${SOURCE}
)

target_include_directories(graffitica PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/src"
	"${SDL3_Headers}"
)

target_link_libraries(graffitica PUBLIC
	SDL3::SDL3
)

add_subdirectory(external)
add_subdirectory(test)
