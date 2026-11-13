cmake_minimum_required(VERSION 3.16)
project(graffitica LANGUAGES CXX)

if(APPLE)
    enable_language(OBJCXX)
    set(CMAKE_OBJCXX_STANDARD 23)
    set(CMAKE_OBJCXX_STANDARD_REQUIRED ON)
endif()

# Set all libs to be static
option(BUILD_SHARED_LIBS "Build dependencies as static libraries" OFF)

# vcpkg must be selected before project() by the user (for example with
# -DCMAKE_TOOLCHAIN_FILE=...). Do not force the bundled Windows-oriented
# toolchain on macOS.

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if (EXISTS "${LOC_PATH}")
  message(FATAL_ERROR "build must be out of source")
endif()

# TODO clean up and split cmakelists between modules later
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

# The D3D12 implementation is Windows-only. Keep its files out of the
# macOS target while retaining the complete implementation on Windows.
if(APPLE)
    list(FILTER SOURCES EXCLUDE REGEX "/rhi/d3d12/|/modules/ShaderCompilerModule.cpp$")
endif()

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

if(APPLE)
    target_sources(${PROJECT_NAME} PRIVATE
        src/runtime/rhi/metal/metal_rhi.mm
        src/runtime/rhi/metal/metal_graphics_context.mm)
    set_source_files_properties(
        src/runtime/rhi/metal/metal_rhi.mm
        src/runtime/rhi/metal/metal_graphics_context.mm
        PROPERTIES COMPILE_OPTIONS "-fobjc-arc")
endif()

#file(GLOB_RECURSE SOURCES "src/*.hlsl")
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
#
#        # Disable Visual Studio's automatic HLSL compilation
#        set_source_files_properties(${SHADER} PROPERTIES
#            VS_TOOL_OVERRIDE "None"
#            HEADER_FILE_ONLY TRUE
#        )
#    endforeach()
#)


# Allow for VS cpu instrumentation
if (MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/PROFILE")
endif()

# TODO split to different cmake?
if(NOT APPLE)
    find_package(Vulkan REQUIRED COMPONENTS glslc shaderc_combined dxc)
endif()

# Point to your Vulkan SDK installation
#set(VULKAN_SDK $ENV{VULKAN_SDK})

target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/runtime"
    "${SDL3_Headers}"
    Tracy::TracyClient
)

if(NOT APPLE)
    target_include_directories(${PROJECT_NAME} PUBLIC Vulkan::Headers)
endif()

#find_package(directx-headers CONFIG REQUIRED)
#find_package(directx12-agility CONFIG REQUIRED)

# Vulkan SDK root
if(WIN32)
set(VULKAN_SDK $ENV{VULKAN_SDK})

# Slang paths inside Vulkan SDK
set(VULKAN_INCLUDE_DIR "${VULKAN_SDK}/Include/")
set(VULKAN_LIB_DIR  "${VULKAN_SDK}/Lib")

target_include_directories(graffitica PRIVATE ${VULKAN_INCLUDE_DIR})
target_link_directories(graffitica PRIVATE ${VULKAN_LIB_DIR})
endif()

#find_package(directx-headers CONFIG REQUIRED)
#find_package(directx12-agility CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
    #Microsoft::DirectX12-Agility

    xxHash::xxhash

    fastgltf

    Tracy::TracyClient
)

if(NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        Vulkan::Vulkan
        Vulkan::shaderc_combined
        slang
        slang-compiler
    )
    target_compile_definitions(${PROJECT_NAME} PRIVATE GRAFFITICA_HAS_SLANG=1)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE GRAFFITICA_METAL=1)
endif()

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        d3d12 dxgi dxguid d3dcompiler Microsoft::DirectX-Headers)
endif()

if(APPLE)
    foreach(METAL_FRAMEWORK Metal QuartzCore Foundation)
        find_library(${METAL_FRAMEWORK}_FRAMEWORK ${METAL_FRAMEWORK} REQUIRED)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${${METAL_FRAMEWORK}_FRAMEWORK})
    endforeach()
endif()

if(TARGET Microsoft::DirectX12-Agility)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/D3D12"
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PROPERTY:Microsoft::DirectX12-Core,IMPORTED_LOCATION_RELEASE> $<TARGET_FILE_DIR:${PROJECT_NAME}>/D3D12
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_PROPERTY:Microsoft::DirectX12-Layers,IMPORTED_LOCATION_DEBUG> $<TARGET_FILE_DIR:${PROJECT_NAME}>/D3D12
    )
endif()


target_compile_definitions(${PROJECT_NAME} PRIVATE
    NOMINMAX
)

target_compile_options(${PROJECT_NAME} PRIVATE
    #$<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Werror>
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Werror>
)

# OpenMP
if(APPLE)
    # AppleClang does not ship OpenMP runtime support by default. The current
    # CPU rasterizer remains usable without OpenMP on macOS.
    find_package(OpenMP QUIET)
else()
    find_package(OpenMP REQUIRED)
endif()
if (OPENMP_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
    target_compile_options(${PROJECT_NAME} PRIVATE
        ${OpenMP_CXX_FLAGS} # Or OpenMP_C_FLAGS, OpenMP_Fortran_FLAGS
    )
endif()

#find_package(directx12-agility CONFIG REQUIRED)

add_subdirectory(external)
add_subdirectory(test)
add_subdirectory(src/shaders)
add_dependencies(${PROJECT_NAME} CompileShaders)

add_dependencies(${PROJECT_NAME} CopyShaders)


