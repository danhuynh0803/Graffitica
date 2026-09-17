#pragma once

#include <fastgltf/core.hpp>

#include <fastgltf/types.hpp>
#include <fastgltf/tools.hpp>

#include <filesystem>
#include <iostream>
#include "renderer/mesh.h" // MeshAttributes

// Copied just for reference on error codes
//enum class Error : std::uint64_t {
//    None = 0,
//    InvalidPath = 1, ///< The glTF directory passed to load*GLTF is invalid.
//    MissingExtensions = 2, ///< One or more extensions are required by the glTF but not enabled in the Parser.
//    UnknownRequiredExtension = 3, ///< An extension required by the glTF is not supported by fastgltf.
//    InvalidJson = 4, ///< An error occurred while parsing the JSON.
//    InvalidGltf = 5, ///< The glTF is either missing something or has invalid data.
//    InvalidOrMissingAssetField = 6, ///< The glTF asset object is missing or invalid.
//    InvalidGLB = 7, ///< The GLB container is invalid.
//    /**
//     * A field is missing in the JSON.
//     * @note This is only used internally.
//     */
//    MissingField = 8,
//    MissingExternalBuffer = 9, ///< With Options::LoadExternalBuffers, an external buffer was not found.
//    UnsupportedVersion = 10, ///< The glTF version is not supported by fastgltf.
//    InvalidURI = 11, ///< A URI from a buffer or image failed to be parsed.
//    InvalidFileData = 12, ///< The file data is invalid, or the file type could not be determined.
//    FailedWritingFiles = 13, ///< The exporter failed to write some files (buffers/images) to disk.
//    FileBufferAllocationFailed = 14, ///< The constructor of GltfDataBuffer failed to allocate a sufficiently large buffer.
//};

inline const char* ToString(fastgltf::Error err)
{
    switch (err) {
    case fastgltf::Error::None:                         return "None";
    case fastgltf::Error::InvalidPath:                  return "InvalidPath";
    case fastgltf::Error::MissingExtensions:            return "MissingExtensions";
    case fastgltf::Error::UnknownRequiredExtension:     return "UnknownRequiredExtension";
    case fastgltf::Error::InvalidJson:                  return "InvalidJson";
    case fastgltf::Error::InvalidGltf:                  return "InvalidGltf";
    case fastgltf::Error::InvalidOrMissingAssetField:   return "InvalidOrMissingAssetField";
    case fastgltf::Error::InvalidGLB:                   return "InvalidGLB";
    case fastgltf::Error::MissingField:                 return "MissingField";
    case fastgltf::Error::MissingExternalBuffer:        return "MissingExternalBuffer";
    case fastgltf::Error::UnsupportedVersion:           return "UnsupportedVersion";
    case fastgltf::Error::InvalidURI:                   return "InvalidURI";
    case fastgltf::Error::InvalidFileData:              return "InvalidFileData";
    case fastgltf::Error::FailedWritingFiles:           return "FailedWritingFiles";
    case fastgltf::Error::FileBufferAllocationFailed:   return "FileBufferAllocationFailed";
    default:                                            return "UnknownError";
    }
}

MeshAttributes LoadGLTFMesh(const std::filesystem::path& path)
{
    MeshAttributes outMesh;

    // Initialize fastgltf parser
    fastgltf::Parser parser;

    // Configure options (e.g., load external buffers automatically)
    constexpr auto options = fastgltf::Options::LoadExternalBuffers;

    auto data = fastgltf::GltfDataBuffer::FromPath(path);
    if (data.error() != fastgltf::Error::None) {
        std::cerr << "Failed to load glTF file data.\n";
        return outMesh;
    }

    auto asset = parser.loadGltf(data.get(), path.parent_path(), options);
    if (asset.error() != fastgltf::Error::None) {
        std::cerr << "Failed to parse glTF: " << ToString(asset.error()) << "\n";
        return outMesh;
    }

    // Grab the first primitive of the first mesh for simplification
    if (asset->meshes.empty() || asset->meshes[0].primitives.empty()) return outMesh;

    const auto& primitive = asset->meshes[0].primitives[0];

    // --- 1. Extract Indices ---
    if (primitive.indicesAccessor.has_value()) {
        const auto& accessor = asset->accessors[primitive.indicesAccessor.value()];
        outMesh.indices.reserve(accessor.count);

        fastgltf::iterateAccessor<uint32_t>(asset.get(), accessor, [&](uint32_t index) {
            outMesh.indices.push_back(index);
        });
    }

    // --- 2. Extract Vertex Positions ---
    auto posAttribute = primitive.findAttribute("POSITION");
    if (posAttribute != primitive.attributes.end()) {
        const auto& accessor = asset->accessors[posAttribute->accessorIndex];
        outMesh.positions.reserve(accessor.count * 3); // 3 floats per position

        fastgltf::iterateAccessor<fastgltf::math::f32vec3>(asset.get(), accessor, [&](fastgltf::math::f32vec3 pos) {
            outMesh.positions.push_back(pos.x());
            outMesh.positions.push_back(pos.y());
            outMesh.positions.push_back(pos.z());
        });
    }

    // --- 3. Extract Vertex Normals ---
    auto normalAttribute = primitive.findAttribute("NORMAL");
    if (normalAttribute != primitive.attributes.end()) {
        const auto& accessor = asset->accessors[normalAttribute->accessorIndex];
        fastgltf::iterateAccessor<fastgltf::math::f32vec3>(asset.get(), accessor, [&](fastgltf::math::f32vec3 normal) {
            outMesh.normals.push_back(normal.x());
            outMesh.normals.push_back(normal.y());
            outMesh.normals.push_back(normal.z());
        });
    }

    // --- 4. Extract Vertex UVs ---
    auto uvAttribute = primitive.findAttribute("TEXCOORD_0");
    if (uvAttribute != primitive.attributes.end()) {
        const auto& accessor = asset->accessors[uvAttribute->accessorIndex];
        fastgltf::iterateAccessor<fastgltf::math::f32vec2>(asset.get(), accessor, [&](fastgltf::math::f32vec2 uv) {
            outMesh.uvs.push_back(uv.x());
            outMesh.uvs.push_back(uv.y());
        });
    }

    return outMesh;
}