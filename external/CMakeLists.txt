# External vendor libraries

#target_compile_definitions(
#    glfw
#    PUBLIC
#        -DGLFW_INCLUDE_NONE
#        #-DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\"
#)

# imgui
#add_subdirectory(imgui)
#set_target_properties(imgui PROPERTIES FOLDER "external")

# maybe use later
# spdlog
#add_library(spdlog INTERFACE)
#target_include_directories(spdlog INTERFACE "spdlog/include/")

# maybe use or create my own
# stb
#add_library(stb INTERFACE)
#target_include_directories(stb INTERFACE "stb/")
#set_target_properties(stb PROPERTIES FOLDER "Vendor")

# sdl
add_subdirectory(SDL)
#set_target_properties(sdl PROPERTIES FOLDER "external")

add_subdirectory(Catch2)

add_subdirectory(tracy)

add_subdirectory(DirectX-Headers)


set(AGILITY_SDK_VER 1.619.2)
set(AGILITY_SDK_DIR ${CMAKE_SOURCE_DIR}/external/D3D12AgilitySDK/${AGILITY_SDK_VER}/bin/x64/)
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/D3D12/)

set(AGILITY_DIR ${CMAKE_SOURCE_DIR}/external/D3D12AgilitySDK/${AGILITY_SDK_VER}/bin/)
if (TARGET_ARCH STREQUAL X86)
    set(bin_path ${AGILITY_DIR}/win32)
elseif (TARGET_ARCH STREQUAL X64)
    set(bin_path ${AGILITY_DIR}/x64)
elseif (TARGET_ARCH STREQUAL ARM)
    set(bin_path ${AGILITY_DIR}/arm)
elseif (TARGET_ARCH STREQUAL ARM64)
    set(bin_path ${AGILITY_DIR}/arm64)
endif()



#install(FILES
#    ${bin_path}/D3D12Core.dll
#    ${bin_path}/D3D12SDKLayers.dll
#    ${bin_path}/D3D12SDKVersion
#    ${bin_path}/D3D12SDKPath
#    DESTINATION .
#)

#include(FetchContent)
#set(content ${PROJECT_NAME}_content)
#set(pkg_id Microsoft.Direct3D.D3D12)
#set(pkg_version 1.619.2)
#FetchContent_Declare(
#    ${content}
#    URL "https://www.nuget.org/api/v2/package/${pkg_id}/${pkg_version}"
#    #URL_HASH SHA256=${pkg_hash}
#)
#FetchContent_MakeAvailable(${content})

#target_append_redist_file(${PROJECT_NAME} "${bin_path}/D3D12Core.dll" "D3D12/D3D12Core.dll")
#target_append_redist_file(${PROJECT_NAME} "${bin_path}/d3d12SDKLayers.dll" "D3D12/d3d12SDKLayers.dll")
#target_append_redist_file(${PROJECT_NAME} "${bin_path}/d3dconfig.exe" "D3D12/d3dconfig.exe")

#add_custom_command(OUTPUT CopyD3D12AgilityDLLs POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        ${bin_path}/D3D12Core.dll
#        ${OUTPUT_DIR}
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        ${bin_path}/D3D12SDKLayers.dll
#        ${OUTPUT_DIR}
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        ${bin_path}/D3D12SDKVersion
#        ${OUTPUT_DIR}
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        ${bin_path}/D3D12SDKPath
#        ${OUTPUT_DIR}
#)
