#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <core/types.h>
#include <stdexcept>

// TODO
// Using an intermediate ImageData struct to avoid modifying RHICreateTexture that uses TextureDesc
// Usage will be to assign ImageData values to TextureDesc for now
struct ImageData
{
    ImageData(const char* srcFilePath)
    {
        pData = stbi_load(srcFilePath, &width, &height, &channels, 4);

        if (!pData) {
            std::string res = "Could not load image: " + std::string(srcFilePath) + " " + stbi_failure_reason();
            throw std::runtime_error(res.c_str());
        }
    }

    ~ImageData()
    {
        stbi_image_free(pData);
    }

    int width = 0, height = 0, channels = 0;
    unsigned char* pData = nullptr;
};

void ReadImage(const char* srcFilePath, void* pDataDst)
{
    int width, height, channels;
    unsigned char* data = stbi_load("image.png", &width, &height, &channels, 4);

    if (!data) {
        throw std::runtime_error("Could not load image");
    }

    stbi_image_free(data);
}

void WriteImage(const char* dstFilePath)
{
    // TODO
}
