#pragma once

#include <slang/slang.h>
#include <slang/slang-gfx.h>
#include <d3d12.h>
#include <memory>
#include <wrl.h>
#include <vector>
#include <wchar.h>

namespace gr
{

using Microsoft::WRL::ComPtr;

struct ShaderOutputs
{
    ComPtr<slang::IBlob> VS;
    ComPtr<slang::IBlob> PS;
    ComPtr<slang::IBlob> CS;


    //ComPtr<ID3DBlob> dxilVS;
    //ComPtr<ID3DBlob> dxilPS;
    //ComPtr<ID3DBlob> dxilCS;

    std::vector<UINT8> spirvVS;
    std::vector<UINT8> spirvPS;
    std::vector<UINT8> spirvCS;
};

class ShaderCompilerModule
{
public:
    ShaderCompilerModule();

    ShaderOutputs CompileSlangToBlob(const char* filePath, const char* entryPoint);
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