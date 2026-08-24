cmake_policy(SET CMP0048 NEW)

set(PROJECT_NAME graffitica)

# TODO Win Platform
# Set for using vcpkg
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/external/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

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
find_package(Vulkan COMPONENTS  glslc
                                shaderc_combined
                                dxc
)

# Point to your Vulkan SDK installation
#set(VULKAN_SDK $ENV{VULKAN_SDK})

target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/runtime"
    "${SDL3_Headers}"
    Vulkan::Headers
    Tracy::TracyClient
)

#find_package(directx-headers CONFIG REQUIRED)
#find_package(directx12-agility CONFIG REQUIRED)

# Vulkan SDK root
set(VULKAN_SDK $ENV{VULKAN_SDK})

# Slang paths inside Vulkan SDK
set(VULKAN_INCLUDE_DIR "${VULKAN_SDK}/Include/")
set(VULKAN_LIB_DIR  "${VULKAN_SDK}/Lib")

target_include_directories(graffitica PRIVATE ${VULKAN_INCLUDE_DIR})
target_link_directories(graffitica PRIVATE ${VULKAN_LIB_DIR})

#find_package(directx-headers CONFIG REQUIRED)
#find_package(directx12-agility CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
    Vulkan::Vulkan
    Vulkan::shaderc_combined

    slang
    slang-compiler

    d3d12
    dxgi
    dxguid
    d3dcompiler
    Microsoft::DirectX-Headers
    #Microsoft::DirectX12-Agility

    Tracy::TracyClient
)

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

#find_package(directx12-agility CONFIG REQUIRED)

add_subdirectory(external)
add_subdirectory(test)
add_subdirectory(src/shaders)
add_dependencies(${PROJECT_NAME} CompileShaders)
