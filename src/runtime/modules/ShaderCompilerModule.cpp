#include "ShaderCompilerModule.h"
#include <slang/slang-com-ptr.h>
#include <slang/slang-com-helper.h>
#include <stdexcept>
#include <vector>
#include "rhi/interface/rhi.h"

namespace gr
{


slang::TargetDesc ShaderCompilerModule::GetSlangTargetDesc(SlangCompileTarget targetFormat, const char* profileName)
{
    slang::TargetDesc targetDesc {};
    targetDesc.format = targetFormat;
    targetDesc.profile = mGlobalSession->findProfile(profileName);
    return targetDesc;
}

void DiagnoseIfNeeded(slang::IBlob* diagnosticsBlob)
{
    if (diagnosticsBlob != nullptr)
    {
        printf("%s", (const char*)diagnosticsBlob->getBufferPointer());
    }
}

ShaderCompilerModule::ShaderCompilerModule()
{
    slang::createGlobalSession(mGlobalSession.writeRef());

    // TODO support multiple targets, for now we just support one target

    slang::SessionDesc sessionDesc {};
    slang::TargetDesc targetDescs[RHI_BACKEND::COUNT];
    targetDescs[RHI_BACKEND::D3D12].format = SLANG_DXIL;
    targetDescs[RHI_BACKEND::D3D12].profile = mGlobalSession->findProfile("sm_6_0");

    targetDescs[RHI_BACKEND::VULKAN].format = SLANG_SPIRV;
    targetDescs[RHI_BACKEND::VULKAN].profile = mGlobalSession->findProfile("spirv_1_5");
    
    targetDescs[RHI_BACKEND::CPU].format = SLANG_CPP_SOURCE;
    // Is a "profile" needed for compiling to CPP src? No documentation found on it
    //targetDescs[RHI_BACKEND::CPU].profile = mGlobalSession->findProfile("cpp");

    sessionDesc.targets = targetDescs;
    sessionDesc.targetCount = RHI_BACKEND::COUNT;

    mGlobalSession->createSession(sessionDesc, mSession.writeRef());
}

ShaderOutputs ShaderCompilerModule::CompileSlangToBlob(const char* filePath, const char* entryPointName)
{
    assert(mGlobalSession != nullptr);
    assert(mSession != nullptr);
    Slang::ComPtr<slang::IBlob> diagnosticsBlob;

    // Load the shader code and compile it, unless it exists in memory from the session
    // Note: this method requires tagging entry point functions with [shader("<shaderType>"] attribute
    // e.g. [shader("compute")], "vertex", "fragment", etc
    Slang::ComPtr<slang::IModule> module;
    {
        const char* moduleName = "default";
        module = mSession->loadModule(filePath, diagnosticsBlob.writeRef());
        DiagnoseIfNeeded(diagnosticsBlob);
        if (!module)
        {
            // throw for now TODO
            throw std::runtime_error("Failed to load Slang module");
        }
    }

    // Finding entry points
    Slang::ComPtr<slang::IEntryPoint> vsEntryPoint;
    module->findEntryPointByName("VSMain", vsEntryPoint.writeRef());
    if (!vsEntryPoint) {
        throw std::runtime_error("Failed to load vs entrypoint");
        return {};
    }

    Slang::ComPtr<slang::IEntryPoint> psEntryPoint;
    module->findEntryPointByName("PSMain", psEntryPoint.writeRef());
    if (!psEntryPoint) {
        throw std::runtime_error("Failed to load fs entrypoint");
        return {};
    }

    std::vector<slang::IComponentType*> componentTypes;
    componentTypes.push_back(module);

    // Later on when we go to extract compiled kernel code for our vertex
    // and fragment shaders, we will need to make use of their order within
    // the composition, so we will record the relative ordering of the entry
    // points here as we add them.
    int entryPointCount = 0;
    int vertexEntryPointIndex = entryPointCount++;
    componentTypes.push_back(vsEntryPoint);

    int fragmentEntryPointIndex = entryPointCount++;
    componentTypes.push_back(psEntryPoint);

    // Actually creating the composite component type is a single operation
    // on the Slang session, but the operation could potentially fail if
    // something about the composite was invalid (e.g., you are trying to
    // combine multiple copies of the same module), so we need to deal
    // with the possibility of diagnostic output.
    //
    Slang::ComPtr<slang::IComponentType> composedProgram;
    {
        Slang::ComPtr<slang::IBlob> diagnosticsBlob;
        SlangResult result = mSession->createCompositeComponentType(
            componentTypes.data(),
            componentTypes.size(),
            composedProgram.writeRef(),
            diagnosticsBlob.writeRef());
        DiagnoseIfNeeded(diagnosticsBlob);
        //SLANG_RETURN_ON_FAIL(result);
    }

    Slang::ComPtr<slang::IComponentType> linkedProgram;
    {
        Slang::ComPtr<slang::IBlob> diagnosticsBlob;
        SlangResult result = composedProgram->link(
            linkedProgram.writeRef(),
            diagnosticsBlob.writeRef());
        DiagnoseIfNeeded(diagnosticsBlob);
        //SLANG_RETURN_ON_FAIL(result);
    }

    // TODO what is targetIndex for? Very little documentation found
    SlangInt targetIndex = 0; //SLANG_DXIL;

    Slang::ComPtr<slang::IBlob> vsBlob;
    {
        Slang::ComPtr<slang::IBlob> diagnosticsBlob;
        SlangResult result = linkedProgram->getEntryPointCode(
            vertexEntryPointIndex,
            targetIndex,
            vsBlob.writeRef(),
            diagnosticsBlob.writeRef());
        DiagnoseIfNeeded(diagnosticsBlob);
        //SLANG_RETURN_ON_FAIL(result);
    }

    Slang::ComPtr<slang::IBlob> psBlob;
    {
        Slang::ComPtr<slang::IBlob> diagnosticsBlob;
        SlangResult result = linkedProgram->getEntryPointCode(
            fragmentEntryPointIndex,
            targetIndex,
            psBlob.writeRef(),
            diagnosticsBlob.writeRef());
        DiagnoseIfNeeded(diagnosticsBlob);
        //SLANG_RETURN_ON_FAIL(result);
    }

    ShaderOutputs shaderOutputs {};
    shaderOutputs.VS = vsBlob;
    shaderOutputs.PS = psBlob;





    return shaderOutputs;
}

}
