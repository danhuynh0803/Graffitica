#pragma once

#include <slang/slang.h>
#include <slang/slang-gfx.h>
#include <d3d12.h>
#include <memory>
#include <wrl.h>
#include <vector>
#include <wchar.h>
#include "rhi/interface/rhi.h"

namespace gr
{

using Microsoft::WRL::ComPtr;

struct ShaderOutputs
{
    ComPtr<slang::IBlob> blob;
    //ComPtr<slang::IBlob> VS[RHI_BACKEND::COUNT];
    //ComPtr<slang::IBlob> PS[RHI_BACKEND::COUNT];
    //ComPtr<slang::IBlob> CS[RHI_BACKEND::COUNT];
};

class ShaderCompilerModule
{
public:
    ShaderCompilerModule();

    ShaderOutputs CompileSlangToBlob(RHI_BACKEND desiredBackendToCompile, const char* filePath, const char* entryPointName);

    //ShaderOutputs CompileSlangToBlob(const char* filePath, const char* entryPoint);
    //static Microsoft::WRL::ComPtr<ID3DBlob> CompileSlangToDXIL(const char* filePath, const char* entryPoint, slang::Stage stage);
    //static std::vector<UINT8> CompileSlangToSPIRV(const char* filePath, const char* entryPoint, slang::Stage stage);
    //static std::vector<UINT8> CompileSlangTo(const char* filePath, const char* entryPoint, slang::Stage stage);

private:
    slang::TargetDesc GetSlangTargetDesc(SlangCompileTarget targetFormat, const char* profileName);
private:
    Slang::ComPtr<slang::IGlobalSession> mGlobalSession;
    Slang::ComPtr<slang::ISession> mSession;
};

}