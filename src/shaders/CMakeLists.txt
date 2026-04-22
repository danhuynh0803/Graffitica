# Build HLSL shaders
#
#set_source_files_properties(vertex.hlsl PROPERTIES ShaderType "vs")
#set_source_files_properties(pixel.hlsl PROPERTIES ShaderType "ps")
#
#set(CompileShaders "")
#
#foreach(FILE ${HLSL_SHADER_FILES})
#  get_filename_component(FILE_WE ${FILE} NAME_WE)
#  list(APPEND CSO_SHADER_FILES ${CMAKE_BINARY_DIR}/Shaders/${FILE_WE}.cso)
#  get_source_file_property(shadertype ${FILE} ShaderType)
#  add_custom_target(TARGET CompileShaders PRE_LINK
#      COMMAND dxc.exe /nologo /Emain /T${shadertype}_6_0 $<IF:$<CONFIG:DEBUG>,/Od,/O3> /Zi /Fo ${CMAKE_BINARY_DIR}/Shaders/${FILE_WE}.cso /Fd ${CMAKE_BINARY_DIR}/Shaders/${FILE_WE}.pdb ${FILE}
#                     COMMENT "HLSL ${FILE}"
#                     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#                     VERBATIM)
#endforeach(FILE)
#
#add_custom_target(TARGET CopyShaderBinaries POST_BUILD
#                  COMMAND ${CMAKE_COMMAND} -E copy ${CSO_SHADER_FILES} $<TARGET_FILE_DIR:${PROJECT_NAME}>
#                  COMMAND_EXPAND_LISTS)
# List your shader files

#set(HLSL_SHADERS "")
#
#file(GLOB_RECURSE SOURCES "src/shaders/*.hlsl")
#
#    foreach(SOURCE_FILE ${SOURCES})
#        # Extract the relative path from the source directory
#        string(REPLACE "${CMAKE_SOURCE_DIR}/src/shaders/" "" RELATIVE_PATH "${SOURCE_FILE}")
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
#        list(APPEND HLSL_SHADERS ${SOURCE_FILE})
#        message(STATUS ${SOURCE_FILE})
#    endforeach()

# --- Collect all HLSL shader files ---
#file(GLOB_RECURSE HLSL_SHADERS
#    "${CMAKE_SOURCE_DIR}/src/*.hlsl"
#)
#
## --- Create Visual Studio filters and attach shaders to the target ---
#foreach(SHADER ${HLSL_SHADERS})
#
#    # Compute relative path under src/shaders
#    file(RELATIVE_PATH REL_PATH
#        "${CMAKE_SOURCE_DIR}/src/"
#        "${SHADER}"
#    )
#
#    # Extract folder path (e.g. "lighting/pbr")
#    get_filename_component(SHADER_DIR "${REL_PATH}" DIRECTORY)
#
#    # Convert to VS filter format ("lighting\\pbr")
#    string(REPLACE "/" "\\" VS_FILTER "${SHADER_DIR}")
#
#    # Assign shader to the filter
#    source_group("${VS_FILTER}" FILES "${SHADER}")
#
#    # Disable Visual Studio's automatic HLSL compilation
#    set_source_files_properties(${SHADER} PROPERTIES
#        VS_TOOL_OVERRIDE "None"
#        HEADER_FILE_ONLY TRUE
#    )
#
#endforeach()

# --- Add shaders to your target so they appear in Solution Explorer ---
#target_sources(${PROJECT_NAME} PRIVATE ${HLSL_SHADERS})

# Annotate shader types
#set_source_files_properties(vertex.hlsl PROPERTIES ShaderType "vs")
#set_source_files_properties(pixel.hlsl PROPERTIES ShaderType "ps")

#file(GLOB_RECURSE HLSL_SHADERS "src/shaders/*.hlsl")

set(HLSL_SHADERS triangle.hlsl)

# Output directory for compiled shaders
set(SHADER_OUTPUT_DIR ${CMAKE_BINARY_DIR}/Shaders)

file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})

set(COMPILED_SHADERS "")

foreach(FILE ${HLSL_SHADERS})
    get_filename_component(FILE_WE ${FILE} NAME_WE)
    #get_source_file_property(SHADER_TYPE ${FILE} ShaderType)

    set(PROP "vs")
    set(CSO_OUTPUT ${SHADER_OUTPUT_DIR}/${FILE_WE}_${PROP}.cso)
    list(APPEND COMPILED_SHADERS ${CSO_OUTPUT})

    add_custom_command(
        OUTPUT ${CSO_OUTPUT}
        COMMAND dxc.exe
            /nologo
            #/T${SHADER_TYPE}_6_0
            /T${PROP}_6_0
            /E VSMain
            /Fo ${CSO_OUTPUT}
            /Zi
            ${CMAKE_CURRENT_SOURCE_DIR}/${FILE}
        DEPENDS ${FILE}
        COMMENT "Compiling HLSL shader ${FILE}"
        VERBATIM
    )

    set(PROP "ps")
    set(CSO_OUTPUT ${SHADER_OUTPUT_DIR}/${FILE_WE}_${PROP}.cso)
    list(APPEND COMPILED_SHADERS ${CSO_OUTPUT})
    add_custom_command(
        OUTPUT ${CSO_OUTPUT}
        COMMAND dxc.exe
            /nologo
            /T${PROP}_6_0
            /E PSMain
            /Fo ${CSO_OUTPUT}
            /Zi
            ${CMAKE_CURRENT_SOURCE_DIR}/${FILE}
        DEPENDS ${FILE}
        COMMENT "Compiling HLSL shader ${FILE}"
        VERBATIM
    )
endforeach()

# Custom target that builds all shaders
add_custom_target(CompileShaders ALL
    DEPENDS ${COMPILED_SHADERS}
)
