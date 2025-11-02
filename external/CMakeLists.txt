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
