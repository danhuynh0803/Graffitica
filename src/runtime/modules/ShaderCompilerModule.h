#pragma once

#include <memory>
#include <vector>
#include <wchar.h>
#include "rhi/interface/rhi.h"

#if defined(GRAFFITICA_HAS_SLANG)
#include <slang/slang.h>
#include <slang/slang-gfx.h>
#include <slang/slang-com-ptr.h>
#endif

namespace gr
{

#if defined(GRAFFITICA_HAS_SLANG)
struct ShaderOutputs
{
    Slang::ComPtr<slang::IBlob> blob;
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
#else
struct ShaderOutputs {};

class ShaderCompilerModule
{
public:
    ShaderOutputs CompileSlangToBlob(RHI_BACKEND, const char*, const char*) { return {}; }
};
#endif

}
