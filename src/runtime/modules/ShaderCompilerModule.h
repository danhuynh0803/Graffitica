#pragma once

#include <slang/slang.h>
#include <slang/slang-gfx.h>
#include <d3d12.h>
#include <memory>

namespace gr
{

class ShaderCompilerModule
{
public:
    ShaderCompilerModule();

    std::unique_ptr<slang::IBlob> CompileSlangToBlob(const char* filePath, const char* entryPoint);
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