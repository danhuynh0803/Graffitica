#include "SlangModule.h"

#include <slang/slang-com-ptr.h>
#include <slang/slang-com-helper.h>

namespace gr
{

enum class VERSION
{
    
};

slang::TargetDesc SlangModule::GetSlangTargetDesc(SlangCompileTarget targetFormat, const char* profileName)
{
    slang::TargetDesc targetDesc {};
    targetDesc.format = targetFormat;
    targetDesc.profile = mGlobalSession->findProfile(profileName);
    return targetDesc;
}

SlangModule::SlangModule()
{
    slang::createGlobalSession(mGlobalSession.writeRef());

    // TODO support multiple targets, for now we just support one target
    //std::vector<slang::SessionDesc> sessionDescs = {};
    slang::SessionDesc sessionDesc {};
    slang::TargetDesc targetDesc {};
    targetDesc.format = SLANG_DXIL;
    //targetDesc.profile = globalSession->findProfile("spirv_1_5");
    targetDesc.profile = mGlobalSession->findProfile("sm_6_0");

    sessionDesc.targets = &targetDesc;
    sessionDesc.targetCount = 1;

    Slang::ComPtr<slang::ISession> session;
    mGlobalSession->createSession(sessionDesc, session.writeRef());
}

std::unique_ptr<slang::IBlob> SlangModule::CompileSlangToBlob(const char* filePath, const char* entryPoint)
{
    assert(mGlobalSession != nullptr);
    return std::unique_ptr<slang::IBlob>();
}

}
