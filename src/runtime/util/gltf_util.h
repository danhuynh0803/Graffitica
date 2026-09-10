#pragma once

#include <fastgltf/core.hpp>

#include <fastgltf/types.hpp>
#include <fastgltf/tools.hpp>

#include <filesystem>
#include <iostream>

// Helper structure to hold D3D12-ready mesh data
struct MeshData
{
    std::vector<uint32_t> indices;
    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<float> uvs;
};

MeshData LoadGLTFMesh(const std::filesystem::path& path)
{
    MeshData outMesh;

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
        std::cerr << "Failed to parse glTF: " << fastgltf::to_underlying(asset.error()) << "\n";
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