#pragma once

#include <fastgltf/core.hpp>

#include <fastgltf/types.hpp>
#include <fastgltf/tools.hpp>

#include <filesystem>
#include <iostream>

namespace fg = fastgltf;

void LoadGltfModel(const std::filesystem::path& path)
{
    // Path to your model
    std::filesystem::path path = "model.glb";

    // Configure options
    fg::Parser parser;
    fg::GltfDataBuffer dataBuffer;

    // Load file into memory
    auto loadResult = fg::readFile(path, &dataBuffer);
    if (loadResult != fg::Error::None) {
        std::cerr << "Failed to read glTF file\n";
        return -1;
    }

    // Parse glTF
    auto asset = parser.loadGltf(
        &dataBuffer,
        path.parent_path(),
        fg::Options::LoadExternalBuffers | fg::Options::DecomposeNodeMatrices
    );

    if (asset.error() != fg::Error::None) {
        std::cerr << "Failed to parse glTF\n";
        return -1;
    }

    fg::Asset& gltf = asset.get();

    // Iterate meshes
    for (auto& mesh : gltf.meshes) {
        for (auto& prim : mesh.primitives) {

            // --- Index buffer ---
            if (prim.indicesAccessor.has_value()) {
                const fg::Accessor& indexAcc = gltf.accessors[*prim.indicesAccessor];
                const fg::BufferView& indexView = gltf.bufferViews[indexAcc.bufferView.value()];
                const fg::Buffer& indexBuf = gltf.buffers[indexView.buffer.value()];

                const uint8_t* indexData = indexBuf.data.data() + indexView.byteOffset + indexAcc.byteOffset;
                size_t indexCount = indexAcc.count;

                // Example: cast to uint32_t if needed
                // const uint32_t* indices = reinterpret_cast<const uint32_t*>(indexData);
            }

            // --- Vertex attributes ---
            for (auto& [semantic, accessorIndex] : prim.attributes) {
                const fg::Accessor& acc = gltf.accessors[accessorIndex];
                const fg::BufferView& view = gltf.bufferViews[acc.bufferView.value()];
                const fg::Buffer& buf = gltf.buffers[view.buffer.value()];

                const uint8_t* vertexData = buf.data.data() + view.byteOffset + acc.byteOffset;
                size_t vertexCount = acc.count;

                // Example: POSITION
                if (semantic == fg::Attribute::Position) {
                    // float3 positions
                    // const float* pos = reinterpret_cast<const float*>(vertexData);
                }

                // Example: NORMAL
                if (semantic == fg::Attribute::Normal) {
                    // float3 normals
                }

                // Example: TEXCOORD_0
                if (semantic == fg::Attribute::TexCoord0) {
                    // float2 uvs
                }
            }
        }
    }

    std::cout << "Loaded glTF successfully\n";
    return 0;
}